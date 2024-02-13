// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: MIT-0

import { Construct } from 'constructs';
import {
  SecretValue,
  Stack,
  RemovalPolicy,
  aws_s3,
  aws_secretsmanager,
  aws_iam,
  aws_s3_assets,
  aws_codebuild,
  aws_kms,
  aws_logs,
  aws_sagemaker,
} from 'aws-cdk-lib';
import { BuildTrigger } from '../trigger/build-trigger';

type SageMakerPipelineProps = {
  project: aws_sagemaker.CfnProject;
};

export class SagmakerPipeline extends Construct {
  constructor(scope: Construct, id: string, props: SageMakerPipelineProps) {
    super(scope, id);
    const { project } = props;

    const config = this.node.tryGetContext('config');
    const { region } = Stack.of(this);

    const mlOutputBucket = new aws_s3.Bucket(this, 'MlOutput-har', {
      versioned: true,
      enforceSSL: true,
      encryption: aws_s3.BucketEncryption.S3_MANAGED,
      blockPublicAccess: aws_s3.BlockPublicAccess.BLOCK_ALL,
      removalPolicy: RemovalPolicy.DESTROY,
      autoDeleteObjects: true,
      serverAccessLogsBucket: new aws_s3.Bucket(this, 'MlOutputAccessLog-har', {
        versioned: true,
        enforceSSL: true,
        encryption: aws_s3.BucketEncryption.S3_MANAGED,
        blockPublicAccess: aws_s3.BlockPublicAccess.BLOCK_ALL,
        autoDeleteObjects: true,
        removalPolicy: RemovalPolicy.DESTROY,
      }),
    });

    const mlOpsCode = new aws_s3_assets.Asset(this, 'MlOpsAsset', {
      path: 'mlops',
    });

    const dataSetsBucket = new aws_s3.Bucket(this, 'DataSetsBucket-har', {
      versioned: true,
      enforceSSL: true,
      encryption: aws_s3.BucketEncryption.S3_MANAGED,
      blockPublicAccess: aws_s3.BlockPublicAccess.BLOCK_ALL,
      removalPolicy: RemovalPolicy.DESTROY,
      autoDeleteObjects: true,
      serverAccessLogsBucket: new aws_s3.Bucket(this, 'DataSetsAccessLog-har', {
        versioned: true,
        enforceSSL: true,
        encryption: aws_s3.BucketEncryption.S3_MANAGED,
        blockPublicAccess: aws_s3.BlockPublicAccess.BLOCK_ALL,
        autoDeleteObjects: true,
        removalPolicy: RemovalPolicy.DESTROY,
      }),
    });

    const sageMakerPipelineRole = new aws_iam.Role(this, 'ServiceCatalogProductRole', {
      assumedBy: new aws_iam.CompositePrincipal(
        new aws_iam.ServicePrincipal('sagemaker.amazonaws.com')
      ),
      managedPolicies: [
        aws_iam.ManagedPolicy.fromAwsManagedPolicyName('AmazonSageMakerFullAccess'),
        aws_iam.ManagedPolicy.fromAwsManagedPolicyName('SecretsManagerReadWrite'),
      ],
    });
    dataSetsBucket.grantReadWrite(sageMakerPipelineRole);

    const buildEncryptionKey = new aws_kms.Key(this, 'BuildEncryptionKey', {
      removalPolicy: RemovalPolicy.DESTROY,
      enableKeyRotation: true,
    });
    const build = new aws_codebuild.Project(this, 'MlBuild', {
      encryptionKey: buildEncryptionKey,
      source: aws_codebuild.Source.s3({
        bucket: mlOpsCode.bucket,
        path: mlOpsCode.s3ObjectKey,
      }),
      cache: aws_codebuild.Cache.local(aws_codebuild.LocalCacheMode.SOURCE),
      artifacts: aws_codebuild.Artifacts.s3({
        bucket: mlOutputBucket,
        includeBuildId: false,
        packageZip: false,
      }),
      environmentVariables: {
        SAGEMAKER_PROJECT_NAME: { value: project.projectName },
        SAGEMAKER_PROJECT_ID: { value: project.attrProjectId },
        ARTIFACT_BUCKET: { value: dataSetsBucket.bucketName },
        SAGEMAKER_PIPELINE_NAME: { value: 'sagemaker-' + project.projectName },
        SAGEMAKER_PIPELINE_ROLE_ARN: { value: sageMakerPipelineRole.roleArn },
        AWS_REGION: { value: region },
        STDEVCLOUD_USERNAME_SECRET: { value: config.stUsernameSecret },
        STDEVCLOUD_PASSWORD_SECRET: { value: config.stPasswordSecret },
      },
      buildSpec: aws_codebuild.BuildSpec.fromAsset('lib/ml/buildspec.yml'),
      logging: {
        cloudWatch: {
          logGroup: new aws_logs.LogGroup(this, 'MlBuildLogGroup-har'),
        },
      },
    });
    dataSetsBucket.grantReadWrite(build);
    mlOpsCode.bucket.grantReadWrite(build);
    build.role?.addManagedPolicy(
      aws_iam.ManagedPolicy.fromAwsManagedPolicyName('AmazonSageMakerFullAccess')
    );

    new BuildTrigger(this, 'MlBuildTrigger-har', { buildProject: build, state: mlOpsCode.assetHash });

    const key = new aws_kms.Key(this, 'KMS', {
      removalPolicy: RemovalPolicy.DESTROY,
      enableKeyRotation: true,
    });
    const mlBucketSecret = new aws_secretsmanager.Secret(this, 'MlOutputSecret-har', {
      secretName: 'MlBucketArn',
      secretStringValue: SecretValue.unsafePlainText(mlOutputBucket.bucketArn),
      encryptionKey: key,
    });

    for (const { account } of config.envs.iot) {
      const remotePrincipal = new aws_iam.ArnPrincipal(`arn:aws:iam::${account}:root`);
      key.grantDecrypt(remotePrincipal);
      mlBucketSecret.grantRead(remotePrincipal);
      mlOutputBucket.grantRead(remotePrincipal);
      buildEncryptionKey.grantDecrypt(remotePrincipal);
    }
  }
}
