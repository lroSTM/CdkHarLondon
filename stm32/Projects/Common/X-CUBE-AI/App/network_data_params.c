/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Fri Aug 18 11:15:59 2023
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[398] = {
  0x96161df61b3b343aU, 0xfddef609f3ebc881U, 0x1d0a5ef61726d64U, 0xde365c7f6f0afe2eU,
  0xf8db0a11f603921bU, 0xf07fce28fe011905U, 0xdbff5835e0e2dc81U, 0x1636ece792ee6624U,
  0x1b3358dcbbfdfaa6U, 0x90ae3e61d7fa10eU, 0xfcebd781d8f1d3fcU, 0xff0dfaccecfe04f4U,
  0xf3030b0d1d131520U, 0xfee2f28190deeeedU, 0xd46218d499ebcd68U, 0x2b421e8183dfe535U,
  0x24273336252c2228U, 0x81c3043ee8f22632U, 0x784d43635a481b1aU, 0x1f340e333aee2b7fU,
  0x1704f2edc90e3681U, 0xacef18f2f0081d2bU, 0xe29981e236623b0dU, 0x161a03d9fb1012e9U,
  0xcae8f62c810cdf34U, 0x926221416230d03U, 0xf6eb090808050827U, 0xe2d608b57fc7ffeeU,
  0x322a2d230742f07fU, 0x66221f1715131b22U, 0x27d5e10a6d4b2cb5U, 0xab7f38e1a1ca022fU,
  0x511224627ffcb8afU, 0x6719dbb6f92b051eU, 0x160a214612ee2030U, 0xcf1d0005f50ff181U,
  0xbab2a6e4cde94f39U, 0x81ed6a22153e224dU, 0xf2d01cf6f8da8191U, 0x150d0bf8e5fb00f8U,
  0x7f4f4f5e3fe9e8c8U, 0xc5d7e722ed07fe3eU, 0x5edf2eb1c58017fU, 0x14ede3ccc7e5e4d9U,
  0xafb5c0e3cbfe1e5cU, 0x18ffd0d0c7d5917fU, 0xf5818fb1b990a88aU, 0xdce5cb8f82cbdbebU,
  0xfffff9affffff1f7U, 0x127affffffe1U, 0xfdffffffed4U, 0xfffffa4ffffff763U,
  0xfffffc25fffffbc1U, 0xfffff53efffff3ddU, 0xfffff82afffffad8U, 0x1a1fffff85eU,
  0xcdffffff5cU, 0xfffff24600001757U, 0xfffff865fffffafaU, 0xfffffd15ffffff8bU,
  0xce11ec313d1eb03U, 0xf80dbed431c00cf2U, 0x3ebfeefe915dcd7U, 0x2801f8db09d90204U,
  0xe500defd11cc13dcU, 0xd0daf9e82d0f0dd3U, 0xecfa092006ddf155U, 0xf50d1cfd0f0a0b26U,
  0xf8e8ced01afbee05U, 0x37dd0d0210d201f3U, 0xc1741f007da14ffU, 0xffdf1ffd1717dce5U,
  0x291d0c0925d4240bU, 0xf113d90d1ecb11f5U, 0xf7dbe8155514f001U, 0xd6ec090727bbfd2cU,
  0xec0e1737f42004f5U, 0xe7dbefff1af6e40cU, 0xde001bc91be00a23U, 0x1b36e106da890ddcU,
  0x18eb22fe1606f5c7U, 0xd70d24e7fab5f135U, 0x15f5d7eefee7f0cdU, 0xf7f3e8fc3efddbc7U,
  0xc91c121906e5e842U, 0xe90ed62f26f720e6U, 0xf9e503de1a0be0f0U, 0x215d80fd903f511U,
  0xf7192b21f92dea10U, 0xf1092b1224d702fcU, 0xb12f40cdc14ee07U, 0xf10d12000f24f20bU,
  0xc16140bf2e6f5f5U, 0x15f4d206d213fefdU, 0xf2e3b03f30cddf8U, 0xf82408071df6e4ffU,
  0x17f2ec09d111ed0eU, 0xf0131e10fe17f603U, 0xf6f7100512f4e8f7U, 0xff01d211d5fffe0eU,
  0xf4071c1e092dfbfdU, 0x7f70c0de3cefafbU, 0x20feccf7e817f2fdU, 0xfc22250d1e1afaecU,
  0xf71c09041206f9fbU, 0x514de07e20cfbffU, 0x2121b1d011dff0cU, 0xe90d281a18e8e3f8U,
  0x12f3e91aff18fbfeU, 0xfefe1823f814040cU, 0xe60714150be8f3f6U, 0x14fae2f5f61309f2U,
  0xff3233f5060df30bU, 0x917150ffcf80416U, 0x605db08fb06ed11U, 0x3103014f022eff2U,
  0xf20f370314df0401U, 0x1511d60af912ef0eU, 0xf0fb1306062bf303U, 0xc0e1406fcd5fae9U,
  0x1f04c103d318faefU, 0x32b2bfb000ef5f7U, 0xfe22040b08f3e308U, 0x11fde1fcda0df8fdU,
  0xe5150814fd2bf7f4U, 0xf8fb12120be10302U, 0xf50ff109e704f709U, 0x3fd141a031bf30bU,
  0xc1a1600d707ecU, 0x1f02d602d81efaf8U, 0xfc2a260d1b17efd9U, 0x716060603f1f5fcU,
  0xfa03d618fe13fc06U, 0xff071e1afd25010dU, 0xf4182a180ccafdf2U, 0x9f4d01ce709fdf2U,
  0x511141e09161010U, 0xe711031416e2eef8U, 0x2608e3fdfe0900fdU, 0x31b380bfa0cf50fU,
  0x70b160e0be6fb16U, 0x4fd20ed17f7eef1U, 0xfb01f2e40d0c22e5U, 0xfa0a05ea161404edU,
  0xe43b08f51307e205U, 0xf113fefc132a21f6U, 0xf4f8e7f8fa2005e8U, 0xeb072a0413e9dc3aU,
  0xe42e0610312c1121U, 0xde0fd0d85b0febf1U, 0x4081ced2301fa07U, 0x9f9e70f282df0U,
  0xe9080cfb101500e6U, 0xee2a0c0107fcf4ffU, 0xf617f4fefe1109f5U, 0xfc08e5fb10030d00U,
  0xea1d0a0c01e5db4eU, 0xea31221932331109U, 0xd10fd3d75f0becfeU, 0x9031ce90601f107U,
  0xfef702ecf21122e6U, 0xf40b04f5060017f0U, 0xeb2307f810fae8efU, 0xe717060c012211f4U,
  0xfb0ee0fc0b02fce6U, 0x137f90711f3d447U, 0xf627161b3e251019U, 0xe303c6e24217f4e8U,
  0xf3fff00bdc0b0e07U, 0x40f0b2300e8e013U, 0x8ebf216f8fa0519U, 0x16cae51208fb1bfdU,
  0xae70509f8d8d810U, 0x30a1a05f3faea17U, 0xceeedf0ef1327caU, 0x1ddef9f0c5d4e7e4U,
  0x24eb3827a4f51608U, 0xfdfbe215eef90bf4U, 0xfeea1121f9dfd419U, 0x11f1f10df5defb16U,
  0xecfe3f208050f00U, 0x10ec08040ceae911U, 0xafa1f03eff0f9ffU, 0x10e7eaf3f41425bcU,
  0x21d2e5e4cccef1f8U, 0x2aeb3029a4ea2708U, 0xf9f4fc14e50915f7U, 0x4fdf91a0de4d910U,
  0xdf0fe0cf001e810U, 0x1ad1e9fe0704180aU, 0xbf302eefee6f917U, 0x4ff1704f7fff420U,
  0x2bef2fdfe102bb7U, 0x8d7edefc7e3ebe4U, 0x23fd3f20c2eb0317U, 0xcfb13f322f4ef02U,
  0xfbf8fad9030e1ee1U, 0xf30506f21c2afee1U, 0xed351ef102ffe308U, 0xfd170000083321f0U,
  0xf0efeb01092c08e8U, 0xf0141b1104e6d536U, 0xe32f130f38300f1eU, 0xde03d0dd6621eef3U,
  0x7ff15ee15faf413U, 0xfc1804ec093320f4U, 0xea0605ee141307eaU, 0xf02c05fd07f3f404U,
  0xfe15020aff1703f3U, 0xf302e20219fd02f9U, 0xf2130a06ffe9e041U, 0xe42d34102e341101U,
  0xdcfddfdb640ddbf2U, 0x60f15ed0dfbeb11U, 0xfa040ceaf40c27e4U, 0xf00003f61c1a17ebU,
  0xef1e14f803f9edefU, 0xf313081601210cecU, 0xfefce8f6150303e9U, 0xfc35180610eddb46U,
  0xfa2517173a2a1619U, 0xdbf3cdd8440bf6e6U, 0xf3ef1bf01ff815f3U, 0xf7f0e4e204f014edU,
  0x2e4dcedf418fdf6U, 0xe825e320ec0effU, 0xb02f4fcfdf009feU, 0xf7e2eefd0726fdfcU,
  0xe11231fb2df0f91cU, 0xfae7c80508fa270cU, 0xf0dbe9f1ea0e0bfdU, 0xf90f2aff1fe60a0cU,
  0x20e6f3ee02ef1301U, 0xbf6f8f004370802U, 0xfc0629e530fa04feU, 0xa10f5f3f5d105fbU,
  0xaf0f3e42137f815U, 0xe80a3bf719ecfb11U, 0xfad2efefebf50b0fU, 0xf3f8f40013120003U,
  0xf6012ade20e609f2U, 0x5e2f9ef00e317f6U, 0x1f1e9dafd1e12faU, 0xf10520e21ee9ef06U,
  0x1217eee60ff6ecf2U, 0x16edeae7012d1907U, 0xdd0b14070deff315U, 0xfdf0d20e06f010faU,
  0xfdeadbe5010ef3e7U, 0xf7060006e9061405U, 0xb010426fae9e023U, 0x9f7fb11e8d8fc16U,
  0x14d0ed0efffb14fbU, 0x13e70107f3cde408U, 0x410120704f5ea18U, 0x11f4daf5ec1627caU,
  0x1bdbf1eeccd4f1e7U, 0x26f23023a8e9160eU, 0x2fbe109eb040df8U, 0xfdf20a12ffd9db13U,
  0x18f9f50ee4de0919U, 0x10d5e3f6fb080dfdU, 0xce7fcfb04e5fc12U, 0xd011802eaf5f507U,
  0x12e8f901011d27bbU, 0x13d3d9f1d4cbf2fbU, 0x24fb292ea6e6190fU, 0xf6f6fd12f5020df3U,
  0x3f613fceedc19U, 0x6f1040cedf6f316U, 0xfe2f701fe0a190aU, 0x7e6f5e9fee4ef0aU,
  0x902130df703f819U, 0x5caf6ffe41520bbU, 0x8e2eee7c9d3e6e7U, 0x28fe2f20c6f60713U,
  0x310ed12de07ea14U, 0xfe112621f430f205U, 0xf813320f24dd19ffU, 0x1212cf18e016fb02U,
  0xf2f609111e14e310U, 0xf1d1906f0cfeff5U, 0x2bfcc702dd15faf2U, 0x121e33fff908ece5U,
  0xfa300d1016dae3faU, 0x1506bf0ce116f006U, 0xe22d0b19fd1df011U, 0xf809fe0cfae6f500U,
  0xf6ffd41bdc0e0414U, 0xecfd2107f01ffe0aU, 0xfa0d191ee3c004fbU, 0x270bd302ea2203f1U,
  0x42a2f011204f6f5U, 0xc1902fc05f70503U, 0xff02ed21d51ff203U, 0x12021c24f026f203U,
  0xe7132a2510d9e5f8U, 0x14fbc52ede1cfef5U, 0xfef60f17021f0407U, 0xe909111601d304f9U,
  0x27f1c608ec1609e2U, 0x42d36fbf719ee12U, 0x60a2613fdec0214U, 0xfe07e32f0d2c24eeU,
  0xeeb650dd65612f8U, 0xb2c001cfefd202cU, 0xe5f9df19ff0feaf3U, 0x10e837f1ec620d1fU,
  0x1a1925fdcafbe70eU, 0x3001f5f4051efecdU, 0x4fac90efdfeeef4U, 0xee250b2d00f50dfbU,
  0xd912dcd7fe2de7f0U, 0x7d4c1f7ff38d8eaU, 0xfe3adaead2e41b2cU, 0xdbe5faebf737e101U,
  0xfa0bfcf4ed36fe02U, 0x122d09eba7e9fffeU, 0x2f1bfced053802dbU, 0x17fff0cb0af3ef0aU,
  0xf2dfdfae21125ffU, 0x1817f130fc26fadcU, 0xe1b916eb1674e711U, 0x7f6ed18d00f0d43U,
  0x1df4f122f14703e2U, 0xfff113260329101fU, 0x1123305d9e81723U, 0x36e9e7f8eb2d05e5U,
  0x1b0733d9e51cf025U, 0x100dee0e06ed0e00U, 0x1b0417c206b2cc09U, 0x718a7d131c0ede3U,
  0xf4c3fa060000e1ccU, 0x2bfb11de2fea0016U, 0xf313d20b21b61df0U, 0xe7d9eae24b0f01ecU,
  0xe3fc241af8d10043U, 0xed030dfb18f81321U, 0xfecee0cf2affefedU, 0x1ef52d0f1cdc17feU,
  0xfe3a2405fcc813fcU, 0x6d831100210e4dfU, 0x1b26111332c7201dU, 0xe72ce5070bbfece4U,
  0xf6c3d60649151019U, 0xd8e00f1115b40438U, 0x505123be422090cU, 0xcfe20af92105d3f3U,
  0xe90804c712d40d01U, 0xf26dd0af28a29cfU, 0x16e31ed8331b11d5U, 0xd30c0ee1e4c3d828U,
  0x25fed8ec1af4fcd2U, 0xee0dc0426f5eacaU, 0xc005041f10c8092eU, 0xfd05cd131afe15d6U,
  0xe30402e809e8d1efU, 0xf60cf237022f1deaU, 0xbf76f12b13c150cU, 0xc1a0701fdeb1027U,
  0xeb06ee1ce82ddb05U, 0xdff3cd5fc3d0c1aU, 0xc2202ffc7f8f11cU, 0x221beddfef1f05c6U,
  0x601f913fff9ffedU, 0x535ff20dff60c22U, 0xc715edfddb1ddbf0U, 0xbdee3f8f036e2dfU,
  0xf120f1e5f2fd2823U, 0xe4f2f4fdd946ddccU, 0xae6fdf3e05316fdU, 0x152a21f3b6fd16f3U,
  0x424505fced2dfff4U, 0x7eafad71402eb12U, 0x121ef912fe1504f3U, 0x2d0eec1aee2cf1f6U,
  0xf0e731e32a7fdc2aU, 0xf903cf10dcf10d42U, 0x1d0b0a2e0a4e13dfU, 0xe3f21d17c71d1018U,
  0xec0b16f8dce71c26U, 0x4509f1deef26fadaU, 0x5fc3df4f81cf837U, 0x1003e9f806161907U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0xc8e515ec0e300c04U, 0xe07e36f16190c31U,
  0x8f7f8c0bc3111aefU, 0xd00c0df4080ce608U, 0xb90de705111d0810U, 0x131b0b121f09c140U,
  0x140fffffda7U, 0x7300000155U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

