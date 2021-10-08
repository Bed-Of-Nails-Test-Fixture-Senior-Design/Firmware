/*
This file contains calibration data for the Amplifier Test Fixture. See the comments
in CalPcb.ino for a description of the data.
*/

// Conversion and calibration values. Last factor is board-specific.
// For Due by itself. 
//#define V_TO_Q  (4096.0/3.3*3.0/2.0 * 1.0)  // Use for generating signals (units are Q/V)
//#define Q_TO_V  (3.3/4096.0 * 1.0)          // Use for receiving signals (units are (V/Q)

// For Due in the test fixture.
#define V_TO_Q  (4096.0/3.3*3.0/2.0 * (91.0+150.0)/150.0 * 0.994)  // Use for generating signals. (units are Q/V)
#define Q_TO_V  (3.3/4096.0 * 180.0/22.0 * 0.9815)          // Use for receiving signals. (units are (V/Q)


/* The data below is frequency response data for the Amplifier Test Fixture.
 *  It represents the cascaded analog response normalized to 0 dB at 990.5 Hz.
 *  See the comments in CalHwFreqResp.ino for a description of the data.
 */

#define HW_CAL_TABLE_SIZE 223
#define HW_CAL_TABLE { \
{21.50, -0.55},   \
{43.10, -0.18},   \
{64.60, -0.07},   \
{86.10, -0.03},   \
{107.70, -0.02},  \
{129.20, -0.01},  \
{150.70,  0.01},  \
{172.30, -0.01},  \
{193.80, -0.04},  \
{215.30, -0.02},  \
{236.90,  0.02},  \
{258.40, -0.04},  \
{279.90,  0.01},  \
{301.50, -0.04},  \
{323.00,  0.02},  \
{344.50, -0.04},  \
{366.10,  0.02},  \
{387.60,  0.00},  \
{409.10, -0.02},  \
{430.70,  0.02},  \
{452.20,  0.00},  \
{473.70, -0.04},  \
{495.30,  0.02},  \
{516.80,  0.04},  \
{538.30,  0.00},  \
{559.90,  0.02},  \
{581.40, -0.02},  \
{602.90,  0.00},  \
{624.50, -0.03},  \
{646.00,  0.01},  \
{667.50, -0.02},  \
{689.10, -0.03},  \
{710.60,  0.01},  \
{732.10,  0.00},  \
{753.70,  0.00},  \
{775.20,  0.05},  \
{796.70,  0.01},  \
{818.30, -0.03},  \
{839.80,  0.03},  \
{861.30,  0.06},  \
{882.90,  0.02},  \
{904.40,  0.01},  \
{925.90,  0.03},  \
{947.50, -0.02},  \
{969.00, -0.00},  \
{990.50,  0.01},  \
{1012.10, -0.01},   \
{1033.60,  0.01},   \
{1055.10, -0.01},   \
{1076.70,  0.02},   \
{1098.20,  0.01},   \
{1119.70,  0.05},   \
{1141.30,  0.05},   \
{1162.80,  0.01},   \
{1184.30,  0.01},   \
{1205.90,  0.05},   \
{1227.40,  0.07},   \
{1248.90,  0.02},   \
{1270.50,  0.05},   \
{1292.00,  0.02},   \
{1313.50,  0.02},   \
{1335.10,  0.01},   \
{1356.60,  0.04},   \
{1378.10,  0.01},   \
{1399.70,  0.07},   \
{1421.20,  0.03},   \
{1442.70,  0.01},   \
{1464.30,  0.09},   \
{1485.80,  0.06},   \
{1507.30,  0.04},   \
{1528.90,  0.03},   \
{1550.40,  0.03},   \
{1571.90,  0.10},   \
{1593.50,  0.09},   \
{1615.00,  0.09},   \
{1636.50,  0.08},   \
{1658.10,  0.09},   \
{1679.60,  0.09},   \
{1701.10,  0.02},   \
{1722.70,  0.02},   \
{1744.20,  0.05},   \
{1765.70,  0.07},   \
{1787.30,  0.10},   \
{1808.80,  0.03},   \
{1830.30,  0.06},   \
{1873.40,  0.05},   \
{1894.90,  0.10},   \
{1916.50,  0.06},   \
{1938.00,  0.09},   \
{1959.50,  0.05},   \
{1981.10,  0.12},   \
{2002.60,  0.06},   \
{2024.10,  0.05},   \
{2067.20,  0.08},   \
{2088.70,  0.07},   \
{2110.30,  0.03},   \
{2131.80,  0.11},   \
{2174.90,  0.10},   \
{2196.40,  0.12},   \
{2217.90,  0.10},   \
{2239.50,  0.08},   \
{2282.50,  0.09},   \
{2304.10,  0.14},   \
{2325.60,  0.12},   \
{2368.70,  0.09},   \
{2390.20,  0.09},   \
{2433.30,  0.15},   \
{2454.80,  0.08},   \
{2476.30,  0.11},   \
{2519.40,  0.07},   \
{2540.90,  0.09},   \
{2584.00,  0.14},   \
{2605.50,  0.17},   \
{2648.60,  0.17},   \
{2670.10,  0.16},   \
{2713.20,  0.11},   \
{2756.20,  0.15},   \
{2777.80,  0.15},   \
{2820.80,  0.12},   \
{2863.90,  0.12},   \
{2885.40,  0.15},   \
{2928.50,  0.15},   \
{2971.60,  0.15},   \
{2993.10,  0.15},   \
{3036.20,  0.17},   \
{3079.20,  0.20},   \
{3122.30,  0.17},   \
{3143.80,  0.15},   \
{3186.90,  0.14},   \
{3230.00,  0.15},   \
{3273.00,  0.17},   \
{3316.10,  0.20},   \
{3359.20,  0.16},   \
{3402.20,  0.22},   \
{3445.30,  0.21},   \
{3488.40,  0.19},   \
{3531.40,  0.21},   \
{3574.50,  0.21},   \
{3617.60,  0.19},   \
{3660.60,  0.23},   \
{3703.70,  0.20},   \
{3768.30,  0.20},   \
{3811.40,  0.21},   \
{3854.40,  0.23},   \
{3897.50,  0.25},   \
{3962.10,  0.23},   \
{4005.20,  0.17},   \
{4048.20,  0.25},   \
{4112.80,  0.18},   \
{4155.90,  0.24},   \
{4199.00,  0.25},   \
{4263.60,  0.23},   \
{4306.60,  0.23},   \
{4371.20,  0.21},   \
{4414.30,  0.29},   \
{4478.90,  0.22},   \
{4543.50,  0.23},   \
{4586.60,  0.24},   \
{4651.20,  0.26},   \
{4715.80,  0.23},   \
{4758.80,  0.24},   \
{4823.40,  0.27},   \
{4888.00,  0.26},   \
{4952.60,  0.25},   \
{5017.20,  0.30},   \
{5081.80,  0.27},   \
{5146.40,  0.21},   \
{5211.00,  0.25},   \
{5275.60,  0.24},   \
{5340.20,  0.27},   \
{5404.80,  0.21},   \
{5469.40,  0.19},   \
{5534.00,  0.22},   \
{5620.20,  0.22},   \
{5684.80,  0.20},   \
{5749.40,  0.20},   \
{5835.50,  0.17},   \
{5900.10,  0.18},   \
{5986.20,  0.17},   \
{6050.80,  0.15},   \
{6137.00,  0.16},   \
{6201.60,  0.13},   \
{6287.70,  0.18},   \
{6352.30,  0.17},   \
{6438.40,  0.15},   \
{6524.60,  0.08},   \
{6610.70,  0.07},   \
{6696.80,  0.07},   \
{6783.00,  0.00},   \
{6869.10, -0.03},   \
{6955.20, -0.04},   \
{7041.40, -0.08},   \
{7127.50, -0.14},   \
{7213.60, -0.17},   \
{7299.80, -0.16},   \
{7385.90, -0.20},   \
{7493.60, -0.24},   \
{7579.70, -0.38},   \
{7687.40, -0.41},   \
{7773.50, -0.48},   \
{7881.20, -0.50},   \
{7967.30, -0.59},   \
{8075.00, -0.76},   \
{8182.60, -0.81},   \
{8268.80, -0.89},   \
{8376.40, -1.02},   \
{8484.10, -1.18},   \
{8591.70, -1.27},   \
{8699.40, -1.45},   \
{8807.10, -1.59},   \
{8914.70, -1.85},   \
{9043.90, -2.05},   \
{9151.60, -2.29},   \
{9259.30, -2.50},   \
{9388.50, -2.66},   \
{9496.10, -3.03},   \
{9625.30, -3.32},   \
{9733.00, -3.61},   \
{9862.20, -3.94},   \
{9991.40, -4.40},   \
{10120.60, -4.69},  \
{10249.80, -5.22},  \
{10379.00, -5.58}   \
}