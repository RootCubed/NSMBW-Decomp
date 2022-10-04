#include <types.h>
#include <lib/MSL_C/math/fabs.h>
#include <lib/MSL_C/math/fmod.h>
#include <dol/cLib/c_random.hpp>
/// @file

/// @cond
#define M_2PI 6.2831854820251465
/// @endcond

static cM_rand_c s_rnd = cM_rand_c(100); ///< [Used for tile randomization, eye animations and a few more things].
static cM_rand_c s_rnd2 = cM_rand_c(101); ///< [Unused].

/// @cond
static u16 atntable[1025] = { 0x0, 0xA, 0x14, 0x1F, 0x29, 0x33, 0x3D, 0x47, 0x51, 0x5C, 0x66, 0x70, 0x7A,
0x84, 0x8F, 0x99, 0xA3, 0xAD, 0xB7, 0xC2, 0xCC, 0xD6, 0xE0, 0xEA, 0xF4, 0xFF, 0x109, 0x113, 0x11D,
0x127, 0x131, 0x13C, 0x146, 0x150, 0x15A, 0x164, 0x16F, 0x179, 0x183, 0x18D, 0x197, 0x1A1, 0x1AC,
0x1B6, 0x1C0, 0x1CA, 0x1D4, 0x1DE, 0x1E9, 0x1F3, 0x1FD, 0x207, 0x211, 0x21B, 0x226, 0x230, 0x23A,
0x244, 0x24E, 0x258, 0x262, 0x26D, 0x277, 0x281, 0x28B, 0x295, 0x29F, 0x2A9, 0x2B4, 0x2BE, 0x2C8,
0x2D2, 0x2DC, 0x2E6, 0x2F0, 0x2FB, 0x305, 0x30F, 0x319, 0x323, 0x32D, 0x337, 0x341, 0x34C, 0x356,
0x360, 0x36A, 0x374, 0x37E, 0x388, 0x392, 0x39C, 0x3A7, 0x3B1, 0x3BB, 0x3C5, 0x3CF, 0x3D9, 0x3E3,
0x3ED, 0x3F7, 0x401, 0x40C, 0x416, 0x420, 0x42A, 0x434, 0x43E, 0x448, 0x452, 0x45C, 0x466, 0x470,
0x47A, 0x484, 0x48E, 0x499, 0x4A3, 0x4AD, 0x4B7, 0x4C1, 0x4CB, 0x4D5, 0x4DF, 0x4E9, 0x4F3, 0x4FD,
0x507, 0x511, 0x51B, 0x525, 0x52F, 0x539, 0x543, 0x54D, 0x557, 0x561, 0x56B, 0x575, 0x57F, 0x589,
0x593, 0x59D, 0x5A7, 0x5B1, 0x5BB, 0x5C5, 0x5CF, 0x5D9, 0x5E3, 0x5ED, 0x5F7, 0x601, 0x60B, 0x615,
0x61F, 0x629, 0x633, 0x63D, 0x647, 0x651, 0x65B, 0x665, 0x66E, 0x678, 0x682, 0x68C, 0x696, 0x6A0,
0x6AA, 0x6B4, 0x6BE, 0x6C8, 0x6D2, 0x6DC, 0x6E5, 0x6EF, 0x6F9, 0x703, 0x70D, 0x717, 0x721, 0x72B,
0x735, 0x73E, 0x748, 0x752, 0x75C, 0x766, 0x770, 0x77A, 0x783, 0x78D, 0x797, 0x7A1, 0x7AB, 0x7B5,
0x7BE, 0x7C8, 0x7D2, 0x7DC, 0x7E6, 0x7EF, 0x7F9, 0x803, 0x80D, 0x817, 0x820, 0x82A, 0x834, 0x83E,
0x848, 0x851, 0x85B, 0x865, 0x86F, 0x878, 0x882, 0x88C, 0x896, 0x89F, 0x8A9, 0x8B3, 0x8BD, 0x8C6,
0x8D0, 0x8DA, 0x8E3, 0x8ED, 0x8F7, 0x901, 0x90A, 0x914, 0x91E, 0x927, 0x931, 0x93B, 0x944, 0x94E,
0x958, 0x961, 0x96B, 0x975, 0x97E, 0x988, 0x992, 0x99B, 0x9A5, 0x9AE, 0x9B8, 0x9C2, 0x9CB, 0x9D5,
0x9DE, 0x9E8, 0x9F2, 0x9FB, 0xA05, 0xA0E, 0xA18, 0xA22, 0xA2B, 0xA35, 0xA3E, 0xA48, 0xA51, 0xA5B,
0xA64, 0xA6E, 0xA77, 0xA81, 0xA8B, 0xA94, 0xA9E, 0xAA7, 0xAB1, 0xABA, 0xAC4, 0xACD, 0xAD7, 0xAE0,
0xAE9, 0xAF3, 0xAFC, 0xB06, 0xB0F, 0xB19, 0xB22, 0xB2C, 0xB35, 0xB3F, 0xB48, 0xB51, 0xB5B, 0xB64,
0xB6E, 0xB77, 0xB80, 0xB8A, 0xB93, 0xB9D, 0xBA6, 0xBAF, 0xBB9, 0xBC2, 0xBCB, 0xBD5, 0xBDE, 0xBE7,
0xBF1, 0xBFA, 0xC03, 0xC0D, 0xC16, 0xC1F, 0xC29, 0xC32, 0xC3B, 0xC45, 0xC4E, 0xC57, 0xC60, 0xC6A,
0xC73, 0xC7C, 0xC86, 0xC8F, 0xC98, 0xCA1, 0xCAB, 0xCB4, 0xCBD, 0xCC6, 0xCCF, 0xCD9, 0xCE2, 0xCEB,
0xCF4, 0xCFD, 0xD07, 0xD10, 0xD19, 0xD22, 0xD2B, 0xD34, 0xD3E, 0xD47, 0xD50, 0xD59, 0xD62, 0xD6B,
0xD74, 0xD7D, 0xD87, 0xD90, 0xD99, 0xDA2, 0xDAB, 0xDB4, 0xDBD, 0xDC6, 0xDCF, 0xDD8, 0xDE1, 0xDEA,
0xDF3, 0xDFC, 0xE05, 0xE0F, 0xE18, 0xE21, 0xE2A, 0xE33, 0xE3C, 0xE45, 0xE4E, 0xE56, 0xE5F, 0xE68,
0xE71, 0xE7A, 0xE83, 0xE8C, 0xE95, 0xE9E, 0xEA7, 0xEB0, 0xEB9, 0xEC2, 0xECB, 0xED4, 0xEDC, 0xEE5,
0xEEE, 0xEF7, 0xF00, 0xF09, 0xF12, 0xF1B, 0xF23, 0xF2C, 0xF35, 0xF3E, 0xF47, 0xF50, 0xF58, 0xF61,
0xF6A, 0xF73, 0xF7C, 0xF84, 0xF8D, 0xF96, 0xF9F, 0xFA7, 0xFB0, 0xFB9, 0xFC2, 0xFCA, 0xFD3, 0xFDC,
0xFE5, 0xFED, 0xFF6, 0xFFF, 0x1007, 0x1010, 0x1019, 0x1021, 0x102A, 0x1033, 0x103B, 0x1044, 0x104D,
0x1055, 0x105E, 0x1067, 0x106F, 0x1078, 0x1080, 0x1089, 0x1092, 0x109A, 0x10A3, 0x10AB, 0x10B4,
0x10BC, 0x10C5, 0x10CE, 0x10D6, 0x10DF, 0x10E7, 0x10F0, 0x10F8, 0x1101, 0x1109, 0x1112, 0x111A,
0x1123, 0x112B, 0x1134, 0x113C, 0x1145, 0x114D, 0x1156, 0x115E, 0x1166, 0x116F, 0x1177, 0x1180,
0x1188, 0x1191, 0x1199, 0x11A1, 0x11AA, 0x11B2, 0x11BB, 0x11C3, 0x11CB, 0x11D4, 0x11DC, 0x11E4,
0x11ED, 0x11F5, 0x11FD, 0x1206, 0x120E, 0x1216, 0x121F, 0x1227, 0x122F, 0x1237, 0x1240, 0x1248,
0x1250, 0x1259, 0x1261, 0x1269, 0x1271, 0x127A, 0x1282, 0x128A, 0x1292, 0x129A, 0x12A3, 0x12AB,
0x12B3, 0x12BB, 0x12C3, 0x12CC, 0x12D4, 0x12DC, 0x12E4, 0x12EC, 0x12F4, 0x12FC, 0x1305, 0x130D,
0x1315, 0x131D, 0x1325, 0x132D, 0x1335, 0x133D, 0x1345, 0x134D, 0x1355, 0x135E, 0x1366, 0x136E,
0x1376, 0x137E, 0x1386, 0x138E, 0x1396, 0x139E, 0x13A6, 0x13AE, 0x13B6, 0x13BE, 0x13C6, 0x13CE,
0x13D6, 0x13DE, 0x13E6, 0x13ED, 0x13F5, 0x13FD, 0x1405, 0x140D, 0x1415, 0x141D, 0x1425, 0x142D,
0x1435, 0x143D, 0x1444, 0x144C, 0x1454, 0x145C, 0x1464, 0x146C, 0x1473, 0x147B, 0x1483, 0x148B,
0x1493, 0x149B, 0x14A2, 0x14AA, 0x14B2, 0x14BA, 0x14C1, 0x14C9, 0x14D1, 0x14D9, 0x14E0, 0x14E8,
0x14F0, 0x14F8, 0x14FF, 0x1507, 0x150F, 0x1516, 0x151E, 0x1526, 0x152D, 0x1535, 0x153D, 0x1544,
0x154C, 0x1554, 0x155B, 0x1563, 0x156B, 0x1572, 0x157A, 0x1581, 0x1589, 0x1591, 0x1598, 0x15A0,
0x15A7, 0x15AF, 0x15B7, 0x15BE, 0x15C6, 0x15CD, 0x15D5, 0x15DC, 0x15E4, 0x15EB, 0x15F3, 0x15FA,
0x1602, 0x1609, 0x1611, 0x1618, 0x1620, 0x1627, 0x162F, 0x1636, 0x163E, 0x1645, 0x164C, 0x1654,
0x165B, 0x1663, 0x166A, 0x1671, 0x1679, 0x1680, 0x1688, 0x168F, 0x1696, 0x169E, 0x16A5, 0x16AC,
0x16B4, 0x16BB, 0x16C2, 0x16CA, 0x16D1, 0x16D8, 0x16E0, 0x16E7, 0x16EE, 0x16F6, 0x16FD, 0x1704,
0x170B, 0x1713, 0x171A, 0x1721, 0x1728, 0x1730, 0x1737, 0x173E, 0x1745, 0x174C, 0x1754, 0x175B,
0x1762, 0x1769, 0x1770, 0x1778, 0x177F, 0x1786, 0x178D, 0x1794, 0x179B, 0x17A2, 0x17AA, 0x17B1,
0x17B8, 0x17BF, 0x17C6, 0x17CD, 0x17D4, 0x17DB, 0x17E2, 0x17E9, 0x17F0, 0x17F7, 0x17FE, 0x1806,
0x180D, 0x1814, 0x181B, 0x1822, 0x1829, 0x1830, 0x1837, 0x183E, 0x1845, 0x184C, 0x1853, 0x185A,
0x1860, 0x1867, 0x186E, 0x1875, 0x187C, 0x1883, 0x188A, 0x1891, 0x1898, 0x189F, 0x18A6, 0x18AD,
0x18B3, 0x18BA, 0x18C1, 0x18C8, 0x18CF, 0x18D6, 0x18DD, 0x18E3, 0x18EA, 0x18F1, 0x18F8, 0x18FF,
0x1906, 0x190C, 0x1913, 0x191A, 0x1921, 0x1928, 0x192E, 0x1935, 0x193C, 0x1943, 0x1949, 0x1950,
0x1957, 0x195D, 0x1964, 0x196B, 0x1972, 0x1978, 0x197F, 0x1986, 0x198C, 0x1993, 0x199A, 0x19A0,
0x19A7, 0x19AE, 0x19B4, 0x19BB, 0x19C2, 0x19C8, 0x19CF, 0x19D5, 0x19DC, 0x19E3, 0x19E9, 0x19F0,
0x19F6, 0x19FD, 0x1A04, 0x1A0A, 0x1A11, 0x1A17, 0x1A1E, 0x1A24, 0x1A2B, 0x1A31, 0x1A38, 0x1A3E,
0x1A45, 0x1A4B, 0x1A52, 0x1A58, 0x1A5F, 0x1A65, 0x1A6C, 0x1A72, 0x1A79, 0x1A7F, 0x1A86, 0x1A8C,
0x1A93, 0x1A99, 0x1A9F, 0x1AA6, 0x1AAC, 0x1AB3, 0x1AB9, 0x1AC0, 0x1AC6, 0x1ACC, 0x1AD3, 0x1AD9,
0x1ADF, 0x1AE6, 0x1AEC, 0x1AF2, 0x1AF9, 0x1AFF, 0x1B05, 0x1B0C, 0x1B12, 0x1B18, 0x1B1F, 0x1B25,
0x1B2B, 0x1B32, 0x1B38, 0x1B3E, 0x1B44, 0x1B4B, 0x1B51, 0x1B57, 0x1B5D, 0x1B64, 0x1B6A, 0x1B70,
0x1B76, 0x1B7D, 0x1B83, 0x1B89, 0x1B8F, 0x1B95, 0x1B9C, 0x1BA2, 0x1BA8, 0x1BAE, 0x1BB4, 0x1BBA,
0x1BC1, 0x1BC7, 0x1BCD, 0x1BD3, 0x1BD9, 0x1BDF, 0x1BE5, 0x1BEB, 0x1BF2, 0x1BF8, 0x1BFE, 0x1C04,
0x1C0A, 0x1C10, 0x1C16, 0x1C1C, 0x1C22, 0x1C28, 0x1C2E, 0x1C34, 0x1C3A, 0x1C40, 0x1C46, 0x1C4C,
0x1C52, 0x1C58, 0x1C5E, 0x1C64, 0x1C6A, 0x1C70, 0x1C76, 0x1C7C, 0x1C82, 0x1C88, 0x1C8E, 0x1C94,
0x1C9A, 0x1CA0, 0x1CA6, 0x1CAC, 0x1CB2, 0x1CB8, 0x1CBE, 0x1CC3, 0x1CC9, 0x1CCF, 0x1CD5, 0x1CDB,
0x1CE1, 0x1CE7, 0x1CED, 0x1CF3, 0x1CF8, 0x1CFE, 0x1D04, 0x1D0A, 0x1D10, 0x1D16, 0x1D1B, 0x1D21,
0x1D27, 0x1D2D, 0x1D33, 0x1D38, 0x1D3E, 0x1D44, 0x1D4A, 0x1D4F, 0x1D55, 0x1D5B, 0x1D61, 0x1D66,
0x1D6C, 0x1D72, 0x1D78, 0x1D7D, 0x1D83, 0x1D89, 0x1D8E, 0x1D94, 0x1D9A, 0x1DA0, 0x1DA5, 0x1DAB,
0x1DB1, 0x1DB6, 0x1DBC, 0x1DC2, 0x1DC7, 0x1DCD, 0x1DD3, 0x1DD8, 0x1DDE, 0x1DE3, 0x1DE9, 0x1DEF,
0x1DF4, 0x1DFA, 0x1DFF, 0x1E05, 0x1E0B, 0x1E10, 0x1E16, 0x1E1B, 0x1E21, 0x1E26, 0x1E2C, 0x1E32,
0x1E37, 0x1E3D, 0x1E42, 0x1E48, 0x1E4D, 0x1E53, 0x1E58, 0x1E5E, 0x1E63, 0x1E69, 0x1E6E, 0x1E74,
0x1E79, 0x1E7F, 0x1E84, 0x1E8A, 0x1E8F, 0x1E94, 0x1E9A, 0x1E9F, 0x1EA5, 0x1EAA, 0x1EB0, 0x1EB5,
0x1EBA, 0x1EC0, 0x1EC5, 0x1ECB, 0x1ED0, 0x1ED5, 0x1EDB, 0x1EE0, 0x1EE6, 0x1EEB, 0x1EF0, 0x1EF6,
0x1EFB, 0x1F00, 0x1F06, 0x1F0B, 0x1F10, 0x1F16, 0x1F1B, 0x1F20, 0x1F26, 0x1F2B, 0x1F30, 0x1F36,
0x1F3B, 0x1F40, 0x1F45, 0x1F4B, 0x1F50, 0x1F55, 0x1F5A, 0x1F60, 0x1F65, 0x1F6A, 0x1F6F, 0x1F75,
0x1F7A, 0x1F7F, 0x1F84, 0x1F8A, 0x1F8F, 0x1F94, 0x1F99, 0x1F9E, 0x1FA4, 0x1FA9, 0x1FAE, 0x1FB3,
0x1FB8, 0x1FBD, 0x1FC3, 0x1FC8, 0x1FCD, 0x1FD2, 0x1FD7, 0x1FDC, 0x1FE1, 0x1FE6, 0x1FEC, 0x1FF1,
0x1FF6, 0x1FFB, 0x2000 };
/// @endcond

namespace cM {

/// @cond
inline float getConst() {
    return 10430.378f; // Equals to 65535 / 2PI
}

inline bool isZero(float val) {
    const int tmp = 0x34000000; // Minimum positive value that satisfies 1.0f + x != 1.0f
    const float F_ULP = *(const float *)&tmp;
    return (fabsf(val) < F_ULP);
}
/// @endcond

s16 rad2s(float rad) {
    int mod = (float)fmod(rad, M_2PI) * getConst();

    if (mod < -0x8000) {
        mod += 0x10000;
    } else if (0x7fff < mod) {
        mod += -0x10000;
    }

    return mod;
}
} // namespace cM

/// @cond
u16 U_GetAtanTable(float sin, float cos) {
    int idx = (sin / cos) * 1024.0f;
    return atntable[idx];
}
/// @endcond

namespace cM {

/// @cond
inline u32 atan2i(float sin,float cos) {

    // 0° or 180°
    if (isZero(sin)) {
        return (cos >= 0.0f) ? 0 : 0x8000;

    // 90° or 270°
    } else if (isZero(cos)) {
        return (sin >= 0.0f) ? 0x4000 : 0xC000;

    } else if (sin >= 0.0f) {

        // Angle between 0° and 45°/45° and 90°
        if (cos >= 0.0f) {
            return (cos >= sin) ? U_GetAtanTable(sin, cos) : 0x4000 - U_GetAtanTable(cos, sin);

        // Angle between 90° and 135°/135° and 180°
        } else {
            return (-cos < sin) ? U_GetAtanTable(-cos, sin) + 0x4000 : 0x8000 - U_GetAtanTable(sin, -cos);
        }

    } else if (cos < 0.0f) {

        // Angle between 180° and 225°
        if (-cos >= -sin) {
            return U_GetAtanTable(-sin, -cos) + 0x8000;

        // Angle between 180° and 270°
        } else {
            return 0xC000 - U_GetAtanTable(-cos, -sin);
        }

    // Angle between 270° and 315°
    } else if (cos < -sin) {
        return U_GetAtanTable(cos, -sin) + 0xC000;

    // Angle between 315° and 360°
    } else {
        return -U_GetAtanTable(-sin, cos);
    }
}
/// @endcond

s16 atan2s(float sin, float cos) {
    return atan2i(sin, cos);
}

void initRnd(ulong seed) {
    s_rnd.mSeed = seed;
}

float rnd() {
    return s_rnd.ranqd2();
}

int rndInt(int max) {
    return max * rnd();
}

float rndF(float max) {
    return max * rnd();
}

} // namespace cM
