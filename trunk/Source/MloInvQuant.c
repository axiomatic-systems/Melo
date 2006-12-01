/*****************************************************************
|
|    Copyright 2004-2006 Axiomatic Systems LLC
|
|    This file is part of Melo (Melo AAC Decoder).
|
|    Unless you have obtained Melo under a difference license,
|    this version of Melo is Melo|GPL.
|    Melo|GPL is free software; you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation; either version 2, or (at your option)
|    any later version.
|
|    Melo|GPL is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with Melo|GPL; see the file COPYING.  If not, write to the
|    Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
|    02111-1307, USA.
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/
#include "MloConfig.h"
#include "MloDebug.h"
#include "MloFloat.h"
#include "MloIndivChnStream.h"
#include "MloInvQuant.h"
#include "MloTypes.h"
#include "MloUtils.h"

/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/
static void MLO_InvQuant_DoInverseQuantisation (MLO_Float coef_arr [], const MLO_Int16 data_arr [], long len);
static inline MLO_Float MLO_InvQuant_ComputePow43 (int quant);

/*----------------------------------------------------------------------
|       Data
+---------------------------------------------------------------------*/
static const MLO_Float MLO_InvQuant_table_pow43 [8192 / 8] =
{
   MLO_FLOAT_C (0),
   MLO_FLOAT_C (16),
   MLO_FLOAT_C (40.3174736),
   MLO_FLOAT_C (69.2279794),
   MLO_FLOAT_C (101.593667),
   MLO_FLOAT_C (136.798076),
   MLO_FLOAT_C (174.443577),
   MLO_FLOAT_C (214.248292),
   MLO_FLOAT_C (256),
   MLO_FLOAT_C (299.532071),
   MLO_FLOAT_C (344.70955),
   MLO_FLOAT_C (391.420496),
   MLO_FLOAT_C (439.570269),
   MLO_FLOAT_C (489.077615),
   MLO_FLOAT_C (539.871867),
   MLO_FLOAT_C (591.890898),
   MLO_FLOAT_C (645.079578),
   MLO_FLOAT_C (699.388593),
   MLO_FLOAT_C (754.773522),
   MLO_FLOAT_C (811.194101),
   MLO_FLOAT_C (868.613637),
   MLO_FLOAT_C (926.998523),
   MLO_FLOAT_C (986.317844),
   MLO_FLOAT_C (1046.54305),
   MLO_FLOAT_C (1107.64767),
   MLO_FLOAT_C (1169.6071),
   MLO_FLOAT_C (1232.39836),
   MLO_FLOAT_C (1296),
   MLO_FLOAT_C (1360.39186),
   MLO_FLOAT_C (1425.55501),
   MLO_FLOAT_C (1491.4716),
   MLO_FLOAT_C (1558.1248),
   MLO_FLOAT_C (1625.49868),
   MLO_FLOAT_C (1693.57813),
   MLO_FLOAT_C (1762.34882),
   MLO_FLOAT_C (1831.79713),
   MLO_FLOAT_C (1901.9101),
   MLO_FLOAT_C (1972.67534),
   MLO_FLOAT_C (2044.08105),
   MLO_FLOAT_C (2116.11594),
   MLO_FLOAT_C (2188.76921),
   MLO_FLOAT_C (2262.03051),
   MLO_FLOAT_C (2335.88991),
   MLO_FLOAT_C (2410.33787),
   MLO_FLOAT_C (2485.36523),
   MLO_FLOAT_C (2560.96318),
   MLO_FLOAT_C (2637.12323),
   MLO_FLOAT_C (2713.83721),
   MLO_FLOAT_C (2791.09723),
   MLO_FLOAT_C (2868.89568),
   MLO_FLOAT_C (2947.2252),
   MLO_FLOAT_C (3026.07869),
   MLO_FLOAT_C (3105.44928),
   MLO_FLOAT_C (3185.33032),
   MLO_FLOAT_C (3265.71536),
   MLO_FLOAT_C (3346.59817),
   MLO_FLOAT_C (3427.97268),
   MLO_FLOAT_C (3509.83303),
   MLO_FLOAT_C (3592.17352),
   MLO_FLOAT_C (3674.98862),
   MLO_FLOAT_C (3758.27294),
   MLO_FLOAT_C (3842.02125),
   MLO_FLOAT_C (3926.22848),
   MLO_FLOAT_C (4010.88967),
   MLO_FLOAT_C (4096),
   MLO_FLOAT_C (4181.55479),
   MLO_FLOAT_C (4267.54946),
   MLO_FLOAT_C (4353.97956),
   MLO_FLOAT_C (4440.84075),
   MLO_FLOAT_C (4528.12879),
   MLO_FLOAT_C (4615.83954),
   MLO_FLOAT_C (4703.96896),
   MLO_FLOAT_C (4792.51313),
   MLO_FLOAT_C (4881.46818),
   MLO_FLOAT_C (4970.83036),
   MLO_FLOAT_C (5060.59599),
   MLO_FLOAT_C (5150.76148),
   MLO_FLOAT_C (5241.32331),
   MLO_FLOAT_C (5332.27803),
   MLO_FLOAT_C (5423.6223),
   MLO_FLOAT_C (5515.35281),
   MLO_FLOAT_C (5607.46633),
   MLO_FLOAT_C (5699.95971),
   MLO_FLOAT_C (5792.82985),
   MLO_FLOAT_C (5886.07372),
   MLO_FLOAT_C (5979.68835),
   MLO_FLOAT_C (6073.67083),
   MLO_FLOAT_C (6168.01829),
   MLO_FLOAT_C (6262.72794),
   MLO_FLOAT_C (6357.79702),
   MLO_FLOAT_C (6453.22284),
   MLO_FLOAT_C (6549.00274),
   MLO_FLOAT_C (6645.13415),
   MLO_FLOAT_C (6741.61449),
   MLO_FLOAT_C (6838.44126),
   MLO_FLOAT_C (6935.61201),
   MLO_FLOAT_C (7033.12431),
   MLO_FLOAT_C (7130.97578),
   MLO_FLOAT_C (7229.16411),
   MLO_FLOAT_C (7327.68697),
   MLO_FLOAT_C (7426.54213),
   MLO_FLOAT_C (7525.72736),
   MLO_FLOAT_C (7625.24048),
   MLO_FLOAT_C (7725.07935),
   MLO_FLOAT_C (7825.24184),
   MLO_FLOAT_C (7925.72589),
   MLO_FLOAT_C (8026.52944),
   MLO_FLOAT_C (8127.65049),
   MLO_FLOAT_C (8229.08705),
   MLO_FLOAT_C (8330.83718),
   MLO_FLOAT_C (8432.89895),
   MLO_FLOAT_C (8535.27047),
   MLO_FLOAT_C (8637.94987),
   MLO_FLOAT_C (8740.93533),
   MLO_FLOAT_C (8844.22504),
   MLO_FLOAT_C (8947.8172),
   MLO_FLOAT_C (9051.71007),
   MLO_FLOAT_C (9155.90192),
   MLO_FLOAT_C (9260.39103),
   MLO_FLOAT_C (9365.17573),
   MLO_FLOAT_C (9470.25437),
   MLO_FLOAT_C (9575.62529),
   MLO_FLOAT_C (9681.2869),
   MLO_FLOAT_C (9787.23759),
   MLO_FLOAT_C (9893.47581),
   MLO_FLOAT_C (10000),
   MLO_FLOAT_C (10106.8086),
   MLO_FLOAT_C (10213.9002),
   MLO_FLOAT_C (10321.2732),
   MLO_FLOAT_C (10428.9263),
   MLO_FLOAT_C (10536.8578),
   MLO_FLOAT_C (10645.0665),
   MLO_FLOAT_C (10753.5508),
   MLO_FLOAT_C (10862.3094),
   MLO_FLOAT_C (10971.341),
   MLO_FLOAT_C (11080.6441),
   MLO_FLOAT_C (11190.2175),
   MLO_FLOAT_C (11300.0597),
   MLO_FLOAT_C (11410.1695),
   MLO_FLOAT_C (11520.5457),
   MLO_FLOAT_C (11631.1868),
   MLO_FLOAT_C (11742.0917),
   MLO_FLOAT_C (11853.259),
   MLO_FLOAT_C (11964.6877),
   MLO_FLOAT_C (12076.3763),
   MLO_FLOAT_C (12188.3239),
   MLO_FLOAT_C (12300.529),
   MLO_FLOAT_C (12412.9907),
   MLO_FLOAT_C (12525.7076),
   MLO_FLOAT_C (12638.6787),
   MLO_FLOAT_C (12751.9028),
   MLO_FLOAT_C (12865.3788),
   MLO_FLOAT_C (12979.1056),
   MLO_FLOAT_C (13093.0821),
   MLO_FLOAT_C (13207.3071),
   MLO_FLOAT_C (13321.7797),
   MLO_FLOAT_C (13436.4987),
   MLO_FLOAT_C (13551.4631),
   MLO_FLOAT_C (13666.6718),
   MLO_FLOAT_C (13782.1239),
   MLO_FLOAT_C (13897.8182),
   MLO_FLOAT_C (14013.7538),
   MLO_FLOAT_C (14129.9297),
   MLO_FLOAT_C (14246.3449),
   MLO_FLOAT_C (14362.9984),
   MLO_FLOAT_C (14479.8893),
   MLO_FLOAT_C (14597.0165),
   MLO_FLOAT_C (14714.3792),
   MLO_FLOAT_C (14831.9764),
   MLO_FLOAT_C (14949.8071),
   MLO_FLOAT_C (15067.8705),
   MLO_FLOAT_C (15186.1655),
   MLO_FLOAT_C (15304.6915),
   MLO_FLOAT_C (15423.4473),
   MLO_FLOAT_C (15542.4322),
   MLO_FLOAT_C (15661.6452),
   MLO_FLOAT_C (15781.0855),
   MLO_FLOAT_C (15900.7523),
   MLO_FLOAT_C (16020.6446),
   MLO_FLOAT_C (16140.7616),
   MLO_FLOAT_C (16261.1026),
   MLO_FLOAT_C (16381.6666),
   MLO_FLOAT_C (16502.4528),
   MLO_FLOAT_C (16623.4605),
   MLO_FLOAT_C (16744.6888),
   MLO_FLOAT_C (16866.1369),
   MLO_FLOAT_C (16987.804),
   MLO_FLOAT_C (17109.6894),
   MLO_FLOAT_C (17231.7922),
   MLO_FLOAT_C (17354.1117),
   MLO_FLOAT_C (17476.6471),
   MLO_FLOAT_C (17599.3977),
   MLO_FLOAT_C (17722.3627),
   MLO_FLOAT_C (17845.5414),
   MLO_FLOAT_C (17968.933),
   MLO_FLOAT_C (18092.5368),
   MLO_FLOAT_C (18216.3521),
   MLO_FLOAT_C (18340.3781),
   MLO_FLOAT_C (18464.6141),
   MLO_FLOAT_C (18589.0595),
   MLO_FLOAT_C (18713.7135),
   MLO_FLOAT_C (18838.5755),
   MLO_FLOAT_C (18963.6446),
   MLO_FLOAT_C (19088.9204),
   MLO_FLOAT_C (19214.402),
   MLO_FLOAT_C (19340.0888),
   MLO_FLOAT_C (19465.9802),
   MLO_FLOAT_C (19592.0754),
   MLO_FLOAT_C (19718.3738),
   MLO_FLOAT_C (19844.8748),
   MLO_FLOAT_C (19971.5778),
   MLO_FLOAT_C (20098.482),
   MLO_FLOAT_C (20225.5868),
   MLO_FLOAT_C (20352.8916),
   MLO_FLOAT_C (20480.3959),
   MLO_FLOAT_C (20608.0989),
   MLO_FLOAT_C (20736),
   MLO_FLOAT_C (20864.0987),
   MLO_FLOAT_C (20992.3943),
   MLO_FLOAT_C (21120.8862),
   MLO_FLOAT_C (21249.5738),
   MLO_FLOAT_C (21378.4566),
   MLO_FLOAT_C (21507.5339),
   MLO_FLOAT_C (21636.8051),
   MLO_FLOAT_C (21766.2698),
   MLO_FLOAT_C (21895.9272),
   MLO_FLOAT_C (22025.7768),
   MLO_FLOAT_C (22155.8182),
   MLO_FLOAT_C (22286.0506),
   MLO_FLOAT_C (22416.4735),
   MLO_FLOAT_C (22547.0865),
   MLO_FLOAT_C (22677.8889),
   MLO_FLOAT_C (22808.8801),
   MLO_FLOAT_C (22940.0597),
   MLO_FLOAT_C (23071.4271),
   MLO_FLOAT_C (23202.9818),
   MLO_FLOAT_C (23334.7232),
   MLO_FLOAT_C (23466.6508),
   MLO_FLOAT_C (23598.7641),
   MLO_FLOAT_C (23731.0625),
   MLO_FLOAT_C (23863.5456),
   MLO_FLOAT_C (23996.2129),
   MLO_FLOAT_C (24129.0637),
   MLO_FLOAT_C (24262.0977),
   MLO_FLOAT_C (24395.3143),
   MLO_FLOAT_C (24528.713),
   MLO_FLOAT_C (24662.2933),
   MLO_FLOAT_C (24796.0548),
   MLO_FLOAT_C (24929.9969),
   MLO_FLOAT_C (25064.1191),
   MLO_FLOAT_C (25198.421),
   MLO_FLOAT_C (25332.9021),
   MLO_FLOAT_C (25467.5619),
   MLO_FLOAT_C (25602.3999),
   MLO_FLOAT_C (25737.4158),
   MLO_FLOAT_C (25872.6089),
   MLO_FLOAT_C (26007.9788),
   MLO_FLOAT_C (26143.5252),
   MLO_FLOAT_C (26279.2474),
   MLO_FLOAT_C (26415.1451),
   MLO_FLOAT_C (26551.2179),
   MLO_FLOAT_C (26687.4652),
   MLO_FLOAT_C (26823.8866),
   MLO_FLOAT_C (26960.4817),
   MLO_FLOAT_C (27097.25),
   MLO_FLOAT_C (27234.1911),
   MLO_FLOAT_C (27371.3046),
   MLO_FLOAT_C (27508.59),
   MLO_FLOAT_C (27646.047),
   MLO_FLOAT_C (27783.6749),
   MLO_FLOAT_C (27921.4736),
   MLO_FLOAT_C (28059.4424),
   MLO_FLOAT_C (28197.5811),
   MLO_FLOAT_C (28335.8892),
   MLO_FLOAT_C (28474.3662),
   MLO_FLOAT_C (28613.0118),
   MLO_FLOAT_C (28751.8256),
   MLO_FLOAT_C (28890.8071),
   MLO_FLOAT_C (29029.956),
   MLO_FLOAT_C (29169.2718),
   MLO_FLOAT_C (29308.7541),
   MLO_FLOAT_C (29448.4026),
   MLO_FLOAT_C (29588.2169),
   MLO_FLOAT_C (29728.1965),
   MLO_FLOAT_C (29868.3411),
   MLO_FLOAT_C (30008.6503),
   MLO_FLOAT_C (30149.1237),
   MLO_FLOAT_C (30289.7609),
   MLO_FLOAT_C (30430.5615),
   MLO_FLOAT_C (30571.5252),
   MLO_FLOAT_C (30712.6516),
   MLO_FLOAT_C (30853.9403),
   MLO_FLOAT_C (30995.3909),
   MLO_FLOAT_C (31137.0031),
   MLO_FLOAT_C (31278.7765),
   MLO_FLOAT_C (31420.7107),
   MLO_FLOAT_C (31562.8054),
   MLO_FLOAT_C (31705.0602),
   MLO_FLOAT_C (31847.4747),
   MLO_FLOAT_C (31990.0486),
   MLO_FLOAT_C (32132.7816),
   MLO_FLOAT_C (32275.6732),
   MLO_FLOAT_C (32418.7232),
   MLO_FLOAT_C (32561.9312),
   MLO_FLOAT_C (32705.2968),
   MLO_FLOAT_C (32848.8196),
   MLO_FLOAT_C (32992.4994),
   MLO_FLOAT_C (33136.3358),
   MLO_FLOAT_C (33280.3285),
   MLO_FLOAT_C (33424.4771),
   MLO_FLOAT_C (33568.7813),
   MLO_FLOAT_C (33713.2407),
   MLO_FLOAT_C (33857.855),
   MLO_FLOAT_C (34002.624),
   MLO_FLOAT_C (34147.5472),
   MLO_FLOAT_C (34292.6243),
   MLO_FLOAT_C (34437.855),
   MLO_FLOAT_C (34583.239),
   MLO_FLOAT_C (34728.7759),
   MLO_FLOAT_C (34874.4655),
   MLO_FLOAT_C (35020.3074),
   MLO_FLOAT_C (35166.3013),
   MLO_FLOAT_C (35312.4469),
   MLO_FLOAT_C (35458.7438),
   MLO_FLOAT_C (35605.1918),
   MLO_FLOAT_C (35751.7905),
   MLO_FLOAT_C (35898.5397),
   MLO_FLOAT_C (36045.439),
   MLO_FLOAT_C (36192.4882),
   MLO_FLOAT_C (36339.6868),
   MLO_FLOAT_C (36487.0347),
   MLO_FLOAT_C (36634.5314),
   MLO_FLOAT_C (36782.1768),
   MLO_FLOAT_C (36929.9705),
   MLO_FLOAT_C (37077.9122),
   MLO_FLOAT_C (37226.0016),
   MLO_FLOAT_C (37374.2385),
   MLO_FLOAT_C (37522.6225),
   MLO_FLOAT_C (37671.1533),
   MLO_FLOAT_C (37819.8307),
   MLO_FLOAT_C (37968.6543),
   MLO_FLOAT_C (38117.624),
   MLO_FLOAT_C (38266.7393),
   MLO_FLOAT_C (38416),
   MLO_FLOAT_C (38565.4058),
   MLO_FLOAT_C (38714.9565),
   MLO_FLOAT_C (38864.6518),
   MLO_FLOAT_C (39014.4913),
   MLO_FLOAT_C (39164.4749),
   MLO_FLOAT_C (39314.6022),
   MLO_FLOAT_C (39464.8729),
   MLO_FLOAT_C (39615.2868),
   MLO_FLOAT_C (39765.8436),
   MLO_FLOAT_C (39916.5431),
   MLO_FLOAT_C (40067.385),
   MLO_FLOAT_C (40218.3689),
   MLO_FLOAT_C (40369.4947),
   MLO_FLOAT_C (40520.762),
   MLO_FLOAT_C (40672.1707),
   MLO_FLOAT_C (40823.7204),
   MLO_FLOAT_C (40975.4109),
   MLO_FLOAT_C (41127.2419),
   MLO_FLOAT_C (41279.2131),
   MLO_FLOAT_C (41431.3244),
   MLO_FLOAT_C (41583.5754),
   MLO_FLOAT_C (41735.9659),
   MLO_FLOAT_C (41888.4956),
   MLO_FLOAT_C (42041.1643),
   MLO_FLOAT_C (42193.9717),
   MLO_FLOAT_C (42346.9176),
   MLO_FLOAT_C (42500.0018),
   MLO_FLOAT_C (42653.2239),
   MLO_FLOAT_C (42806.5837),
   MLO_FLOAT_C (42960.081),
   MLO_FLOAT_C (43113.7156),
   MLO_FLOAT_C (43267.4871),
   MLO_FLOAT_C (43421.3954),
   MLO_FLOAT_C (43575.4402),
   MLO_FLOAT_C (43729.6212),
   MLO_FLOAT_C (43883.9383),
   MLO_FLOAT_C (44038.3912),
   MLO_FLOAT_C (44192.9796),
   MLO_FLOAT_C (44347.7033),
   MLO_FLOAT_C (44502.5621),
   MLO_FLOAT_C (44657.5557),
   MLO_FLOAT_C (44812.6839),
   MLO_FLOAT_C (44967.9465),
   MLO_FLOAT_C (45123.3432),
   MLO_FLOAT_C (45278.8738),
   MLO_FLOAT_C (45434.5382),
   MLO_FLOAT_C (45590.3359),
   MLO_FLOAT_C (45746.2669),
   MLO_FLOAT_C (45902.3308),
   MLO_FLOAT_C (46058.5275),
   MLO_FLOAT_C (46214.8568),
   MLO_FLOAT_C (46371.3184),
   MLO_FLOAT_C (46527.912),
   MLO_FLOAT_C (46684.6376),
   MLO_FLOAT_C (46841.4948),
   MLO_FLOAT_C (46998.4834),
   MLO_FLOAT_C (47155.6032),
   MLO_FLOAT_C (47312.854),
   MLO_FLOAT_C (47470.2356),
   MLO_FLOAT_C (47627.7477),
   MLO_FLOAT_C (47785.3901),
   MLO_FLOAT_C (47943.1627),
   MLO_FLOAT_C (48101.0652),
   MLO_FLOAT_C (48259.0974),
   MLO_FLOAT_C (48417.2591),
   MLO_FLOAT_C (48575.55),
   MLO_FLOAT_C (48733.97),
   MLO_FLOAT_C (48892.5188),
   MLO_FLOAT_C (49051.1963),
   MLO_FLOAT_C (49210.0022),
   MLO_FLOAT_C (49368.9364),
   MLO_FLOAT_C (49527.9985),
   MLO_FLOAT_C (49687.1885),
   MLO_FLOAT_C (49846.5061),
   MLO_FLOAT_C (50005.9511),
   MLO_FLOAT_C (50165.5233),
   MLO_FLOAT_C (50325.2224),
   MLO_FLOAT_C (50485.0484),
   MLO_FLOAT_C (50645.001),
   MLO_FLOAT_C (50805.08),
   MLO_FLOAT_C (50965.2851),
   MLO_FLOAT_C (51125.6163),
   MLO_FLOAT_C (51286.0732),
   MLO_FLOAT_C (51446.6558),
   MLO_FLOAT_C (51607.3638),
   MLO_FLOAT_C (51768.1969),
   MLO_FLOAT_C (51929.1551),
   MLO_FLOAT_C (52090.2381),
   MLO_FLOAT_C (52251.4458),
   MLO_FLOAT_C (52412.7779),
   MLO_FLOAT_C (52574.2342),
   MLO_FLOAT_C (52735.8146),
   MLO_FLOAT_C (52897.5188),
   MLO_FLOAT_C (53059.3467),
   MLO_FLOAT_C (53221.2981),
   MLO_FLOAT_C (53383.3728),
   MLO_FLOAT_C (53545.5706),
   MLO_FLOAT_C (53707.8914),
   MLO_FLOAT_C (53870.3349),
   MLO_FLOAT_C (54032.9009),
   MLO_FLOAT_C (54195.5893),
   MLO_FLOAT_C (54358.3999),
   MLO_FLOAT_C (54521.3324),
   MLO_FLOAT_C (54684.3868),
   MLO_FLOAT_C (54847.5629),
   MLO_FLOAT_C (55010.8604),
   MLO_FLOAT_C (55174.2791),
   MLO_FLOAT_C (55337.819),
   MLO_FLOAT_C (55501.4798),
   MLO_FLOAT_C (55665.2613),
   MLO_FLOAT_C (55829.1634),
   MLO_FLOAT_C (55993.1858),
   MLO_FLOAT_C (56157.3285),
   MLO_FLOAT_C (56321.5912),
   MLO_FLOAT_C (56485.9738),
   MLO_FLOAT_C (56650.476),
   MLO_FLOAT_C (56815.0978),
   MLO_FLOAT_C (56979.8388),
   MLO_FLOAT_C (57144.6991),
   MLO_FLOAT_C (57309.6783),
   MLO_FLOAT_C (57474.7764),
   MLO_FLOAT_C (57639.9931),
   MLO_FLOAT_C (57805.3282),
   MLO_FLOAT_C (57970.7817),
   MLO_FLOAT_C (58136.3533),
   MLO_FLOAT_C (58302.0429),
   MLO_FLOAT_C (58467.8503),
   MLO_FLOAT_C (58633.7754),
   MLO_FLOAT_C (58799.8179),
   MLO_FLOAT_C (58965.9777),
   MLO_FLOAT_C (59132.2546),
   MLO_FLOAT_C (59298.6485),
   MLO_FLOAT_C (59465.1593),
   MLO_FLOAT_C (59631.7866),
   MLO_FLOAT_C (59798.5305),
   MLO_FLOAT_C (59965.3906),
   MLO_FLOAT_C (60132.367),
   MLO_FLOAT_C (60299.4593),
   MLO_FLOAT_C (60466.6675),
   MLO_FLOAT_C (60633.9913),
   MLO_FLOAT_C (60801.4306),
   MLO_FLOAT_C (60968.9853),
   MLO_FLOAT_C (61136.6552),
   MLO_FLOAT_C (61304.4402),
   MLO_FLOAT_C (61472.34),
   MLO_FLOAT_C (61640.3546),
   MLO_FLOAT_C (61808.4837),
   MLO_FLOAT_C (61976.7272),
   MLO_FLOAT_C (62145.085),
   MLO_FLOAT_C (62313.5569),
   MLO_FLOAT_C (62482.1427),
   MLO_FLOAT_C (62650.8424),
   MLO_FLOAT_C (62819.6556),
   MLO_FLOAT_C (62988.5824),
   MLO_FLOAT_C (63157.6225),
   MLO_FLOAT_C (63326.7758),
   MLO_FLOAT_C (63496.0421),
   MLO_FLOAT_C (63665.4213),
   MLO_FLOAT_C (63834.9132),
   MLO_FLOAT_C (64004.5177),
   MLO_FLOAT_C (64174.2346),
   MLO_FLOAT_C (64344.0639),
   MLO_FLOAT_C (64514.0052),
   MLO_FLOAT_C (64684.0586),
   MLO_FLOAT_C (64854.2238),
   MLO_FLOAT_C (65024.5007),
   MLO_FLOAT_C (65194.8891),
   MLO_FLOAT_C (65365.3889),
   MLO_FLOAT_C (65536),
   MLO_FLOAT_C (65706.7222),
   MLO_FLOAT_C (65877.5554),
   MLO_FLOAT_C (66048.4994),
   MLO_FLOAT_C (66219.554),
   MLO_FLOAT_C (66390.7192),
   MLO_FLOAT_C (66561.9948),
   MLO_FLOAT_C (66733.3807),
   MLO_FLOAT_C (66904.8766),
   MLO_FLOAT_C (67076.4825),
   MLO_FLOAT_C (67248.1983),
   MLO_FLOAT_C (67420.0237),
   MLO_FLOAT_C (67591.9587),
   MLO_FLOAT_C (67764.0031),
   MLO_FLOAT_C (67936.1568),
   MLO_FLOAT_C (68108.4196),
   MLO_FLOAT_C (68280.7914),
   MLO_FLOAT_C (68453.272),
   MLO_FLOAT_C (68625.8614),
   MLO_FLOAT_C (68798.5593),
   MLO_FLOAT_C (68971.3657),
   MLO_FLOAT_C (69144.2804),
   MLO_FLOAT_C (69317.3033),
   MLO_FLOAT_C (69490.4342),
   MLO_FLOAT_C (69663.673),
   MLO_FLOAT_C (69837.0196),
   MLO_FLOAT_C (70010.4738),
   MLO_FLOAT_C (70184.0355),
   MLO_FLOAT_C (70357.7046),
   MLO_FLOAT_C (70531.4809),
   MLO_FLOAT_C (70705.3644),
   MLO_FLOAT_C (70879.3548),
   MLO_FLOAT_C (71053.452),
   MLO_FLOAT_C (71227.656),
   MLO_FLOAT_C (71401.9665),
   MLO_FLOAT_C (71576.3835),
   MLO_FLOAT_C (71750.9068),
   MLO_FLOAT_C (71925.5363),
   MLO_FLOAT_C (72100.2718),
   MLO_FLOAT_C (72275.1133),
   MLO_FLOAT_C (72450.0606),
   MLO_FLOAT_C (72625.1135),
   MLO_FLOAT_C (72800.2721),
   MLO_FLOAT_C (72975.536),
   MLO_FLOAT_C (73150.9052),
   MLO_FLOAT_C (73326.3796),
   MLO_FLOAT_C (73501.9591),
   MLO_FLOAT_C (73677.6434),
   MLO_FLOAT_C (73853.4326),
   MLO_FLOAT_C (74029.3264),
   MLO_FLOAT_C (74205.3248),
   MLO_FLOAT_C (74381.4275),
   MLO_FLOAT_C (74557.6346),
   MLO_FLOAT_C (74733.9459),
   MLO_FLOAT_C (74910.3612),
   MLO_FLOAT_C (75086.8804),
   MLO_FLOAT_C (75263.5034),
   MLO_FLOAT_C (75440.2301),
   MLO_FLOAT_C (75617.0604),
   MLO_FLOAT_C (75793.9941),
   MLO_FLOAT_C (75971.0311),
   MLO_FLOAT_C (76148.1714),
   MLO_FLOAT_C (76325.4147),
   MLO_FLOAT_C (76502.7609),
   MLO_FLOAT_C (76680.2101),
   MLO_FLOAT_C (76857.7619),
   MLO_FLOAT_C (77035.4163),
   MLO_FLOAT_C (77213.1732),
   MLO_FLOAT_C (77391.0325),
   MLO_FLOAT_C (77568.994),
   MLO_FLOAT_C (77747.0577),
   MLO_FLOAT_C (77925.2233),
   MLO_FLOAT_C (78103.4909),
   MLO_FLOAT_C (78281.8602),
   MLO_FLOAT_C (78460.3313),
   MLO_FLOAT_C (78638.9038),
   MLO_FLOAT_C (78817.5778),
   MLO_FLOAT_C (78996.3531),
   MLO_FLOAT_C (79175.2296),
   MLO_FLOAT_C (79354.2072),
   MLO_FLOAT_C (79533.2858),
   MLO_FLOAT_C (79712.4652),
   MLO_FLOAT_C (79891.7454),
   MLO_FLOAT_C (80071.1262),
   MLO_FLOAT_C (80250.6075),
   MLO_FLOAT_C (80430.1893),
   MLO_FLOAT_C (80609.8713),
   MLO_FLOAT_C (80789.6536),
   MLO_FLOAT_C (80969.5359),
   MLO_FLOAT_C (81149.5181),
   MLO_FLOAT_C (81329.6002),
   MLO_FLOAT_C (81509.7821),
   MLO_FLOAT_C (81690.0636),
   MLO_FLOAT_C (81870.4446),
   MLO_FLOAT_C (82050.925),
   MLO_FLOAT_C (82231.5047),
   MLO_FLOAT_C (82412.1837),
   MLO_FLOAT_C (82592.9617),
   MLO_FLOAT_C (82773.8386),
   MLO_FLOAT_C (82954.8145),
   MLO_FLOAT_C (83135.8891),
   MLO_FLOAT_C (83317.0623),
   MLO_FLOAT_C (83498.3341),
   MLO_FLOAT_C (83679.7043),
   MLO_FLOAT_C (83861.1729),
   MLO_FLOAT_C (84042.7397),
   MLO_FLOAT_C (84224.4046),
   MLO_FLOAT_C (84406.1675),
   MLO_FLOAT_C (84588.0283),
   MLO_FLOAT_C (84769.9869),
   MLO_FLOAT_C (84952.0433),
   MLO_FLOAT_C (85134.1972),
   MLO_FLOAT_C (85316.4486),
   MLO_FLOAT_C (85498.7973),
   MLO_FLOAT_C (85681.2434),
   MLO_FLOAT_C (85863.7866),
   MLO_FLOAT_C (86046.4269),
   MLO_FLOAT_C (86229.1642),
   MLO_FLOAT_C (86411.9983),
   MLO_FLOAT_C (86594.9292),
   MLO_FLOAT_C (86777.9568),
   MLO_FLOAT_C (86961.0809),
   MLO_FLOAT_C (87144.3015),
   MLO_FLOAT_C (87327.6184),
   MLO_FLOAT_C (87511.0316),
   MLO_FLOAT_C (87694.541),
   MLO_FLOAT_C (87878.1464),
   MLO_FLOAT_C (88061.8477),
   MLO_FLOAT_C (88245.6449),
   MLO_FLOAT_C (88429.5379),
   MLO_FLOAT_C (88613.5265),
   MLO_FLOAT_C (88797.6106),
   MLO_FLOAT_C (88981.7902),
   MLO_FLOAT_C (89166.0652),
   MLO_FLOAT_C (89350.4354),
   MLO_FLOAT_C (89534.9008),
   MLO_FLOAT_C (89719.4613),
   MLO_FLOAT_C (89904.1167),
   MLO_FLOAT_C (90088.867),
   MLO_FLOAT_C (90273.712),
   MLO_FLOAT_C (90458.6517),
   MLO_FLOAT_C (90643.686),
   MLO_FLOAT_C (90828.8148),
   MLO_FLOAT_C (91014.0379),
   MLO_FLOAT_C (91199.3553),
   MLO_FLOAT_C (91384.767),
   MLO_FLOAT_C (91570.2727),
   MLO_FLOAT_C (91755.8724),
   MLO_FLOAT_C (91941.566),
   MLO_FLOAT_C (92127.3534),
   MLO_FLOAT_C (92313.2346),
   MLO_FLOAT_C (92499.2093),
   MLO_FLOAT_C (92685.2776),
   MLO_FLOAT_C (92871.4393),
   MLO_FLOAT_C (93057.6944),
   MLO_FLOAT_C (93244.0427),
   MLO_FLOAT_C (93430.4841),
   MLO_FLOAT_C (93617.0186),
   MLO_FLOAT_C (93803.6461),
   MLO_FLOAT_C (93990.3665),
   MLO_FLOAT_C (94177.1796),
   MLO_FLOAT_C (94364.0854),
   MLO_FLOAT_C (94551.0838),
   MLO_FLOAT_C (94738.1747),
   MLO_FLOAT_C (94925.358),
   MLO_FLOAT_C (95112.6337),
   MLO_FLOAT_C (95300.0016),
   MLO_FLOAT_C (95487.4616),
   MLO_FLOAT_C (95675.0137),
   MLO_FLOAT_C (95862.6577),
   MLO_FLOAT_C (96050.3936),
   MLO_FLOAT_C (96238.2213),
   MLO_FLOAT_C (96426.1406),
   MLO_FLOAT_C (96614.1516),
   MLO_FLOAT_C (96802.2541),
   MLO_FLOAT_C (96990.448),
   MLO_FLOAT_C (97178.7333),
   MLO_FLOAT_C (97367.1097),
   MLO_FLOAT_C (97555.5774),
   MLO_FLOAT_C (97744.1361),
   MLO_FLOAT_C (97932.7858),
   MLO_FLOAT_C (98121.5264),
   MLO_FLOAT_C (98310.3578),
   MLO_FLOAT_C (98499.2799),
   MLO_FLOAT_C (98688.2926),
   MLO_FLOAT_C (98877.3959),
   MLO_FLOAT_C (99066.5897),
   MLO_FLOAT_C (99255.8738),
   MLO_FLOAT_C (99445.2482),
   MLO_FLOAT_C (99634.7128),
   MLO_FLOAT_C (99824.2675),
   MLO_FLOAT_C (100013.912),
   MLO_FLOAT_C (100203.647),
   MLO_FLOAT_C (100393.472),
   MLO_FLOAT_C (100583.386),
   MLO_FLOAT_C (100773.39),
   MLO_FLOAT_C (100963.483),
   MLO_FLOAT_C (101153.667),
   MLO_FLOAT_C (101343.939),
   MLO_FLOAT_C (101534.301),
   MLO_FLOAT_C (101724.752),
   MLO_FLOAT_C (101915.293),
   MLO_FLOAT_C (102105.922),
   MLO_FLOAT_C (102296.641),
   MLO_FLOAT_C (102487.448),
   MLO_FLOAT_C (102678.344),
   MLO_FLOAT_C (102869.329),
   MLO_FLOAT_C (103060.403),
   MLO_FLOAT_C (103251.565),
   MLO_FLOAT_C (103442.816),
   MLO_FLOAT_C (103634.155),
   MLO_FLOAT_C (103825.583),
   MLO_FLOAT_C (104017.099),
   MLO_FLOAT_C (104208.703),
   MLO_FLOAT_C (104400.395),
   MLO_FLOAT_C (104592.176),
   MLO_FLOAT_C (104784.044),
   MLO_FLOAT_C (104976),
   MLO_FLOAT_C (105168.044),
   MLO_FLOAT_C (105360.175),
   MLO_FLOAT_C (105552.395),
   MLO_FLOAT_C (105744.701),
   MLO_FLOAT_C (105937.096),
   MLO_FLOAT_C (106129.577),
   MLO_FLOAT_C (106322.146),
   MLO_FLOAT_C (106514.803),
   MLO_FLOAT_C (106707.546),
   MLO_FLOAT_C (106900.376),
   MLO_FLOAT_C (107093.294),
   MLO_FLOAT_C (107286.298),
   MLO_FLOAT_C (107479.389),
   MLO_FLOAT_C (107672.567),
   MLO_FLOAT_C (107865.832),
   MLO_FLOAT_C (108059.183),
   MLO_FLOAT_C (108252.621),
   MLO_FLOAT_C (108446.145),
   MLO_FLOAT_C (108639.756),
   MLO_FLOAT_C (108833.452),
   MLO_FLOAT_C (109027.236),
   MLO_FLOAT_C (109221.105),
   MLO_FLOAT_C (109415.06),
   MLO_FLOAT_C (109609.101),
   MLO_FLOAT_C (109803.229),
   MLO_FLOAT_C (109997.442),
   MLO_FLOAT_C (110191.741),
   MLO_FLOAT_C (110386.125),
   MLO_FLOAT_C (110580.595),
   MLO_FLOAT_C (110775.151),
   MLO_FLOAT_C (110969.792),
   MLO_FLOAT_C (111164.519),
   MLO_FLOAT_C (111359.33),
   MLO_FLOAT_C (111554.228),
   MLO_FLOAT_C (111749.21),
   MLO_FLOAT_C (111944.277),
   MLO_FLOAT_C (112139.429),
   MLO_FLOAT_C (112334.667),
   MLO_FLOAT_C (112529.989),
   MLO_FLOAT_C (112725.396),
   MLO_FLOAT_C (112920.888),
   MLO_FLOAT_C (113116.464),
   MLO_FLOAT_C (113312.125),
   MLO_FLOAT_C (113507.87),
   MLO_FLOAT_C (113703.7),
   MLO_FLOAT_C (113899.614),
   MLO_FLOAT_C (114095.613),
   MLO_FLOAT_C (114291.695),
   MLO_FLOAT_C (114487.862),
   MLO_FLOAT_C (114684.113),
   MLO_FLOAT_C (114880.448),
   MLO_FLOAT_C (115076.867),
   MLO_FLOAT_C (115273.369),
   MLO_FLOAT_C (115469.956),
   MLO_FLOAT_C (115666.626),
   MLO_FLOAT_C (115863.379),
   MLO_FLOAT_C (116060.217),
   MLO_FLOAT_C (116257.138),
   MLO_FLOAT_C (116454.142),
   MLO_FLOAT_C (116651.229),
   MLO_FLOAT_C (116848.4),
   MLO_FLOAT_C (117045.654),
   MLO_FLOAT_C (117242.992),
   MLO_FLOAT_C (117440.412),
   MLO_FLOAT_C (117637.915),
   MLO_FLOAT_C (117835.501),
   MLO_FLOAT_C (118033.171),
   MLO_FLOAT_C (118230.922),
   MLO_FLOAT_C (118428.757),
   MLO_FLOAT_C (118626.674),
   MLO_FLOAT_C (118824.674),
   MLO_FLOAT_C (119022.757),
   MLO_FLOAT_C (119220.921),
   MLO_FLOAT_C (119419.169),
   MLO_FLOAT_C (119617.498),
   MLO_FLOAT_C (119815.91),
   MLO_FLOAT_C (120014.404),
   MLO_FLOAT_C (120212.98),
   MLO_FLOAT_C (120411.638),
   MLO_FLOAT_C (120610.378),
   MLO_FLOAT_C (120809.2),
   MLO_FLOAT_C (121008.104),
   MLO_FLOAT_C (121207.089),
   MLO_FLOAT_C (121406.156),
   MLO_FLOAT_C (121605.305),
   MLO_FLOAT_C (121804.536),
   MLO_FLOAT_C (122003.848),
   MLO_FLOAT_C (122203.241),
   MLO_FLOAT_C (122402.716),
   MLO_FLOAT_C (122602.272),
   MLO_FLOAT_C (122801.909),
   MLO_FLOAT_C (123001.628),
   MLO_FLOAT_C (123201.427),
   MLO_FLOAT_C (123401.308),
   MLO_FLOAT_C (123601.27),
   MLO_FLOAT_C (123801.312),
   MLO_FLOAT_C (124001.435),
   MLO_FLOAT_C (124201.64),
   MLO_FLOAT_C (124401.924),
   MLO_FLOAT_C (124602.29),
   MLO_FLOAT_C (124802.736),
   MLO_FLOAT_C (125003.262),
   MLO_FLOAT_C (125203.869),
   MLO_FLOAT_C (125404.557),
   MLO_FLOAT_C (125605.325),
   MLO_FLOAT_C (125806.173),
   MLO_FLOAT_C (126007.101),
   MLO_FLOAT_C (126208.109),
   MLO_FLOAT_C (126409.197),
   MLO_FLOAT_C (126610.366),
   MLO_FLOAT_C (126811.614),
   MLO_FLOAT_C (127012.942),
   MLO_FLOAT_C (127214.35),
   MLO_FLOAT_C (127415.838),
   MLO_FLOAT_C (127617.406),
   MLO_FLOAT_C (127819.053),
   MLO_FLOAT_C (128020.779),
   MLO_FLOAT_C (128222.585),
   MLO_FLOAT_C (128424.471),
   MLO_FLOAT_C (128626.436),
   MLO_FLOAT_C (128828.48),
   MLO_FLOAT_C (129030.604),
   MLO_FLOAT_C (129232.806),
   MLO_FLOAT_C (129435.088),
   MLO_FLOAT_C (129637.449),
   MLO_FLOAT_C (129839.889),
   MLO_FLOAT_C (130042.408),
   MLO_FLOAT_C (130245.006),
   MLO_FLOAT_C (130447.682),
   MLO_FLOAT_C (130650.437),
   MLO_FLOAT_C (130853.271),
   MLO_FLOAT_C (131056.184),
   MLO_FLOAT_C (131259.175),
   MLO_FLOAT_C (131462.245),
   MLO_FLOAT_C (131665.393),
   MLO_FLOAT_C (131868.619),
   MLO_FLOAT_C (132071.924),
   MLO_FLOAT_C (132275.307),
   MLO_FLOAT_C (132478.769),
   MLO_FLOAT_C (132682.308),
   MLO_FLOAT_C (132885.926),
   MLO_FLOAT_C (133089.621),
   MLO_FLOAT_C (133293.395),
   MLO_FLOAT_C (133497.246),
   MLO_FLOAT_C (133701.176),
   MLO_FLOAT_C (133905.183),
   MLO_FLOAT_C (134109.268),
   MLO_FLOAT_C (134313.43),
   MLO_FLOAT_C (134517.67),
   MLO_FLOAT_C (134721.988),
   MLO_FLOAT_C (134926.383),
   MLO_FLOAT_C (135130.856),
   MLO_FLOAT_C (135335.406),
   MLO_FLOAT_C (135540.033),
   MLO_FLOAT_C (135744.738),
   MLO_FLOAT_C (135949.519),
   MLO_FLOAT_C (136154.378),
   MLO_FLOAT_C (136359.314),
   MLO_FLOAT_C (136564.327),
   MLO_FLOAT_C (136769.418),
   MLO_FLOAT_C (136974.584),
   MLO_FLOAT_C (137179.828),
   MLO_FLOAT_C (137385.149),
   MLO_FLOAT_C (137590.546),
   MLO_FLOAT_C (137796.02),
   MLO_FLOAT_C (138001.571),
   MLO_FLOAT_C (138207.198),
   MLO_FLOAT_C (138412.902),
   MLO_FLOAT_C (138618.682),
   MLO_FLOAT_C (138824.538),
   MLO_FLOAT_C (139030.471),
   MLO_FLOAT_C (139236.481),
   MLO_FLOAT_C (139442.566),
   MLO_FLOAT_C (139648.728),
   MLO_FLOAT_C (139854.965),
   MLO_FLOAT_C (140061.279),
   MLO_FLOAT_C (140267.669),
   MLO_FLOAT_C (140474.135),
   MLO_FLOAT_C (140680.676),
   MLO_FLOAT_C (140887.294),
   MLO_FLOAT_C (141093.987),
   MLO_FLOAT_C (141300.756),
   MLO_FLOAT_C (141507.601),
   MLO_FLOAT_C (141714.521),
   MLO_FLOAT_C (141921.517),
   MLO_FLOAT_C (142128.588),
   MLO_FLOAT_C (142335.735),
   MLO_FLOAT_C (142542.957),
   MLO_FLOAT_C (142750.254),
   MLO_FLOAT_C (142957.627),
   MLO_FLOAT_C (143165.075),
   MLO_FLOAT_C (143372.598),
   MLO_FLOAT_C (143580.197),
   MLO_FLOAT_C (143787.87),
   MLO_FLOAT_C (143995.619),
   MLO_FLOAT_C (144203.442),
   MLO_FLOAT_C (144411.34),
   MLO_FLOAT_C (144619.313),
   MLO_FLOAT_C (144827.361),
   MLO_FLOAT_C (145035.484),
   MLO_FLOAT_C (145243.681),
   MLO_FLOAT_C (145451.953),
   MLO_FLOAT_C (145660.3),
   MLO_FLOAT_C (145868.721),
   MLO_FLOAT_C (146077.216),
   MLO_FLOAT_C (146285.786),
   MLO_FLOAT_C (146494.431),
   MLO_FLOAT_C (146703.149),
   MLO_FLOAT_C (146911.942),
   MLO_FLOAT_C (147120.809),
   MLO_FLOAT_C (147329.751),
   MLO_FLOAT_C (147538.766),
   MLO_FLOAT_C (147747.856),
   MLO_FLOAT_C (147957.019),
   MLO_FLOAT_C (148166.257),
   MLO_FLOAT_C (148375.568),
   MLO_FLOAT_C (148584.953),
   MLO_FLOAT_C (148794.412),
   MLO_FLOAT_C (149003.945),
   MLO_FLOAT_C (149213.551),
   MLO_FLOAT_C (149423.231),
   MLO_FLOAT_C (149632.985),
   MLO_FLOAT_C (149842.812),
   MLO_FLOAT_C (150052.712),
   MLO_FLOAT_C (150262.686),
   MLO_FLOAT_C (150472.734),
   MLO_FLOAT_C (150682.855),
   MLO_FLOAT_C (150893.049),
   MLO_FLOAT_C (151103.316),
   MLO_FLOAT_C (151313.656),
   MLO_FLOAT_C (151524.07),
   MLO_FLOAT_C (151734.556),
   MLO_FLOAT_C (151945.116),
   MLO_FLOAT_C (152155.749),
   MLO_FLOAT_C (152366.454),
   MLO_FLOAT_C (152577.233),
   MLO_FLOAT_C (152788.084),
   MLO_FLOAT_C (152999.008),
   MLO_FLOAT_C (153210.005),
   MLO_FLOAT_C (153421.074),
   MLO_FLOAT_C (153632.216),
   MLO_FLOAT_C (153843.431),
   MLO_FLOAT_C (154054.718),
   MLO_FLOAT_C (154266.077),
   MLO_FLOAT_C (154477.509),
   MLO_FLOAT_C (154689.014),
   MLO_FLOAT_C (154900.59),
   MLO_FLOAT_C (155112.239),
   MLO_FLOAT_C (155323.96),
   MLO_FLOAT_C (155535.754),
   MLO_FLOAT_C (155747.619),
   MLO_FLOAT_C (155959.557),
   MLO_FLOAT_C (156171.566),
   MLO_FLOAT_C (156383.648),
   MLO_FLOAT_C (156595.801),
   MLO_FLOAT_C (156808.027),
   MLO_FLOAT_C (157020.324),
   MLO_FLOAT_C (157232.693),
   MLO_FLOAT_C (157445.134),
   MLO_FLOAT_C (157657.646),
   MLO_FLOAT_C (157870.23),
   MLO_FLOAT_C (158082.886),
   MLO_FLOAT_C (158295.613),
   MLO_FLOAT_C (158508.412),
   MLO_FLOAT_C (158721.282),
   MLO_FLOAT_C (158934.223),
   MLO_FLOAT_C (159147.236),
   MLO_FLOAT_C (159360.32),
   MLO_FLOAT_C (159573.476),
   MLO_FLOAT_C (159786.702),
   MLO_FLOAT_C (160000),
   MLO_FLOAT_C (160213.369),
   MLO_FLOAT_C (160426.809),
   MLO_FLOAT_C (160640.32),
   MLO_FLOAT_C (160853.902),
   MLO_FLOAT_C (161067.555),
   MLO_FLOAT_C (161281.278),
   MLO_FLOAT_C (161495.073),
   MLO_FLOAT_C (161708.938),
   MLO_FLOAT_C (161922.874),
   MLO_FLOAT_C (162136.881),
   MLO_FLOAT_C (162350.958),
   MLO_FLOAT_C (162565.106),
   MLO_FLOAT_C (162779.325),
   MLO_FLOAT_C (162993.614),
   MLO_FLOAT_C (163207.973),
   MLO_FLOAT_C (163422.403),
   MLO_FLOAT_C (163636.904),
   MLO_FLOAT_C (163851.474),
   MLO_FLOAT_C (164066.115),
   MLO_FLOAT_C (164280.826),
   MLO_FLOAT_C (164495.607),
   MLO_FLOAT_C (164710.459),
   MLO_FLOAT_C (164925.38)
};



/*----------------------------------------------------------------------
|       Functions
+---------------------------------------------------------------------*/



/*
==============================================================================
Name: MLO_InvQuant_ProcessChannel
Description:
   Does the inverse quantisation of the spectral data.
   Computes the power table the first time it is called.
   Ref:
      4.6.1.3
Input/output parameters:
	- ics_ptr: Channel data to process. Coefficients are still interleaved.
==============================================================================
*/

void  MLO_InvQuant_ProcessChannel (MLO_IndivChnStream *ics_ptr)
{
   MLO_ASSERT (ics_ptr != NULL);

   MLO_InvQuant_DoInverseQuantisation (
      &ics_ptr->coef_arr [0],
      &ics_ptr->data [0],
      MLO_DEFS_FRAME_LEN_LONG
   );
}



void  MLO_InvQuant_DoInverseQuantisation (MLO_Float coef_arr [], const MLO_Int16 data_arr [], long len)
{
   int            pos;

   MLO_ASSERT (coef_arr != NULL);
   MLO_ASSERT (data_arr != NULL);
   MLO_ASSERT (len > 0);
   MLO_ASSERT (len <= MLO_DEFS_FRAME_LEN_LONG);

   for (pos = 0; pos < len; ++pos)
   {
      MLO_Float      invquant;
      const int      quant = data_arr [pos];

      if (quant < 0)
      {
         invquant = MLO_Float_Neg (MLO_InvQuant_ComputePow43 (-quant));
      }
      else
      {
         invquant = MLO_InvQuant_ComputePow43 (quant);
      }

      coef_arr [pos] = invquant;
   }
}



/*
==============================================================================
Name: MLO_InvQuant_ComputePow43
Description:
   Inverse-quantise the absolute value of the coefficient by raising it to
   the power 4/3.
   Uses a table and interpolate it linearly (for the largest values)
   Requires the table to be already built.
   Ref:
      4.6.1.3
Input parameters:
	- quant: absolute value of the quantised coefficient, range [0 ; 8191]
Returns: The absolute value of the original coefficient, range [0 ; 165113.49]
==============================================================================
*/

MLO_Float   MLO_InvQuant_ComputePow43 (int quant)
{
   MLO_Float      invquant;

   MLO_ASSERT (quant >= 0);
   MLO_ASSERT (quant <= 8191);

#if defined (MLO_CONFIG_FIXED)

	if (quant < MLO_ARRAY_SIZE (MLO_InvQuant_table_pow43))
	{
      invquant = MLO_InvQuant_table_pow43 [quant] >> 4;
	}
	else
	{
		const int         t = quant & 7;
		const int         k = quant >> 3;
		const MLO_Float   r0 = MLO_InvQuant_table_pow43 [k    ];
		const MLO_Float   r1 = MLO_InvQuant_table_pow43 [k + 1];
		invquant = MLO_Float_Lerp (r0, r1, t, 3);

      MLO_ASSERT (k + 1 < MLO_ARRAY_SIZE (MLO_InvQuant_table_pow43));
   }

#else

	if (quant < (int)MLO_ARRAY_SIZE (MLO_InvQuant_table_pow43))
	{
		invquant = MLO_Float_Mul (
         MLO_InvQuant_table_pow43 [quant],
         MLO_FLOAT_C (1.0f / 16)
      );
	}
	else
	{
		const MLO_Float   t = MLO_Float_MulInt (MLO_FLOAT_C (1.0 / 8), quant & 7);
		const int         k = quant >> 3;
		const MLO_Float   r0 = MLO_InvQuant_table_pow43 [k    ];
		const MLO_Float   r1 = MLO_InvQuant_table_pow43 [k + 1];
      const MLO_Float   diff = MLO_Float_Sub (r1, r0);
		invquant = MLO_Float_Add (r0, MLO_Float_Mul (diff, t));

      MLO_ASSERT (k + 1 < (int)MLO_ARRAY_SIZE (MLO_InvQuant_table_pow43));
   }

#endif

   MLO_ASSERT (invquant >= 0);

   return (invquant);
}
