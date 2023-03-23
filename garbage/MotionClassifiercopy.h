#pragma once
#include <stdarg.h>

namespace Eloquent {
    namespace ML {
        namespace Port {
            class SVM {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float kernels[124] = { 0 };
                        float decisions[3] = { 0 };
                        int votes[3] = { 0 };
                        kernels[0] = compute_kernel(x,   234.107781740693 );
                        kernels[1] = compute_kernel(x,   209.395206789073 );
                        kernels[2] = compute_kernel(x,   228.187365375106 );
                        kernels[3] = compute_kernel(x,   194.31433261167 );
                        kernels[4] = compute_kernel(x,   310.104073453035 );
                        kernels[5] = compute_kernel(x,   344.782616081447 );
                        kernels[6] = compute_kernel(x,   310.809518121942 );
                        kernels[7] = compute_kernel(x,   298.477851047818 );
                        kernels[8] = compute_kernel(x,   336.802522174925 );
                        kernels[9] = compute_kernel(x,   306.540183280313 );
                        kernels[10] = compute_kernel(x,   318.837550339576 );
                        kernels[11] = compute_kernel(x,   304.024319177053 );
                        kernels[12] = compute_kernel(x,   311.524791714699 );
                        kernels[13] = compute_kernel(x,   303.937723130519 );
                        kernels[14] = compute_kernel(x,   337.718838196133 );
                        kernels[15] = compute_kernel(x,   340.449203020712 );
                        kernels[16] = compute_kernel(x,   330.829559014473 );
                        kernels[17] = compute_kernel(x,   314.672900858812 );
                        kernels[18] = compute_kernel(x,   319.384972556802 );
                        kernels[19] = compute_kernel(x,   318.376372367674 );
                        kernels[20] = compute_kernel(x,   296.577402904131 );
                        kernels[21] = compute_kernel(x,   312.326752243236 );
                        kernels[22] = compute_kernel(x,   306.173234655297 );
                        kernels[23] = compute_kernel(x,   322.705031840236 );
                        kernels[24] = compute_kernel(x,   309.786584525767 );
                        kernels[25] = compute_kernel(x,   301.310442297938 );
                        kernels[26] = compute_kernel(x,   314.177196995269 );
                        kernels[27] = compute_kernel(x,   318.363971165982 );
                        kernels[28] = compute_kernel(x,   319.72979324924 );
                        kernels[29] = compute_kernel(x,   307.048701263221 );
                        kernels[30] = compute_kernel(x,   312.722265915959 );
                        kernels[31] = compute_kernel(x,   305.037469525351 );
                        kernels[32] = compute_kernel(x,   314.548050770745 );
                        kernels[33] = compute_kernel(x,   317.419006332481 );
                        kernels[34] = compute_kernel(x,   318.6265483269 );
                        kernels[35] = compute_kernel(x,   319.113374447597 );
                        kernels[36] = compute_kernel(x,   297.489206230694 );
                        kernels[37] = compute_kernel(x,   285.015903223307 );
                        kernels[38] = compute_kernel(x,   327.798446901411 );
                        kernels[39] = compute_kernel(x,   331.464307681916 );
                        kernels[40] = compute_kernel(x,   361.606735994796 );
                        kernels[41] = compute_kernel(x,   461.27141265983 );
                        kernels[42] = compute_kernel(x,   342.394032461751 );
                        kernels[43] = compute_kernel(x,   342.969128494926 );
                        kernels[44] = compute_kernel(x,   338.958138146159 );
                        kernels[45] = compute_kernel(x,   329.683141003202 );
                        kernels[46] = compute_kernel(x,   319.852516343134 );
                        kernels[47] = compute_kernel(x,   316.446907343993 );
                        kernels[48] = compute_kernel(x,   302.375899468141 );
                        kernels[49] = compute_kernel(x,   357.471375250842 );
                        kernels[50] = compute_kernel(x,   308.265144759 );
                        kernels[51] = compute_kernel(x,   309.740862193824 );
                        kernels[52] = compute_kernel(x,   299.723175535072 );
                        kernels[53] = compute_kernel(x,   322.306149167867 );
                        kernels[54] = compute_kernel(x,   321.538469778975 );
                        kernels[55] = compute_kernel(x,   304.852648683665 );
                        kernels[56] = compute_kernel(x,   310.942875975449 );
                        kernels[57] = compute_kernel(x,   343.539168567414 );
                        kernels[58] = compute_kernel(x,   340.920593249077 );
                        kernels[59] = compute_kernel(x,   329.625130208038 );
                        kernels[60] = compute_kernel(x,   311.783420215847 );
                        kernels[61] = compute_kernel(x,   323.069296231788 );
                        kernels[62] = compute_kernel(x,   312.158412033869 );
                        kernels[63] = compute_kernel(x,   327.102223270688 );
                        kernels[64] = compute_kernel(x,   311.931403969216 );
                        kernels[65] = compute_kernel(x,   316.894922194045 );
                        kernels[66] = compute_kernel(x,   311.465302090792 );
                        kernels[67] = compute_kernel(x,   315.603824535524 );
                        kernels[68] = compute_kernel(x,   334.240557156776 );
                        kernels[69] = compute_kernel(x,   332.220374409011 );
                        kernels[70] = compute_kernel(x,   328.366275792239 );
                        kernels[71] = compute_kernel(x,   323.622987096684 );
                        kernels[72] = compute_kernel(x,   322.321761691657 );
                        kernels[73] = compute_kernel(x,   326.668511444142 );
                        kernels[74] = compute_kernel(x,   326.707955512412 );
                        kernels[75] = compute_kernel(x,   324.003870618603 );
                        kernels[76] = compute_kernel(x,   321.236582652607 );
                        kernels[77] = compute_kernel(x,   322.999372397718 );
                        kernels[78] = compute_kernel(x,   320.180090070704 );
                        kernels[79] = compute_kernel(x,   339.077541341288 );
                        kernels[80] = compute_kernel(x,   336.270495361798 );
                        kernels[81] = compute_kernel(x,   436.431526793743 );
                        kernels[82] = compute_kernel(x,   317.243527444689 );
                        kernels[83] = compute_kernel(x,   481.136117251161 );
                        kernels[84] = compute_kernel(x,   315.70777045867 );
                        kernels[85] = compute_kernel(x,   301.479042968826 );
                        kernels[86] = compute_kernel(x,   317.48854208915 );
                        kernels[87] = compute_kernel(x,   316.810016444048 );
                        kernels[88] = compute_kernel(x,   319.240601001177 );
                        kernels[89] = compute_kernel(x,   319.840595020828 );
                        kernels[90] = compute_kernel(x,   316.364386481504 );
                        kernels[91] = compute_kernel(x,   324.400278689848 );
                        kernels[92] = compute_kernel(x,   322.825237632863 );
                        kernels[93] = compute_kernel(x,   323.959131705976 );
                        kernels[94] = compute_kernel(x,   329.92530873484 );
                        kernels[95] = compute_kernel(x,   333.650592136663 );
                        kernels[96] = compute_kernel(x,   323.329392107386 );
                        kernels[97] = compute_kernel(x,   326.592806140683 );
                        kernels[98] = compute_kernel(x,   324.184868560568 );
                        kernels[99] = compute_kernel(x,   323.492291406345 );
                        kernels[100] = compute_kernel(x,   334.628887212667 );
                        kernels[101] = compute_kernel(x,   325.445889353463 );
                        kernels[102] = compute_kernel(x,   323.325968381561 );
                        kernels[103] = compute_kernel(x,   322.777615778224 );
                        kernels[104] = compute_kernel(x,   318.044311194458 );
                        kernels[105] = compute_kernel(x,   324.640877696595 );
                        kernels[106] = compute_kernel(x,   317.992817904879 );
                        kernels[107] = compute_kernel(x,   324.548178304788 );
                        kernels[108] = compute_kernel(x,   314.373782221697 );
                        kernels[109] = compute_kernel(x,   372.170989203965 );
                        kernels[110] = compute_kernel(x,   311.485975985895 );
                        kernels[111] = compute_kernel(x,   337.480921319829 );
                        kernels[112] = compute_kernel(x,   344.395507807593 );
                        kernels[113] = compute_kernel(x,   344.947593716001 );
                        kernels[114] = compute_kernel(x,   309.19158269763 );
                        kernels[115] = compute_kernel(x,   320.079912383088 );
                        kernels[116] = compute_kernel(x,   324.206569622938 );
                        kernels[117] = compute_kernel(x,   319.478810950246 );
                        kernels[118] = compute_kernel(x,   341.621535088088 );
                        kernels[119] = compute_kernel(x,   317.194212370343 );
                        kernels[120] = compute_kernel(x,   318.793354336997 );
                        kernels[121] = compute_kernel(x,   310.349598863602 );
                        kernels[122] = compute_kernel(x,   319.855592259515 );
                        kernels[123] = compute_kernel(x,   323.404828819629 );
                        decisions[0] = 0.182901415023
                        + kernels[0] * 0.506094038262
                        + kernels[1] * 0.726847325072
                        + kernels[2] * 0.439148339329
                        + kernels[3] * 0.742449447832
                        + kernels[5]
                        + kernels[8]
                        + kernels[9]
                        + kernels[10]
                        + kernels[11]
                        + kernels[12] * 0.581163913352
                        + kernels[13] * 0.066328750914
                        + kernels[14]
                        + kernels[15]
                        + kernels[16]
                        + kernels[17]
                        + kernels[18]
                        + kernels[19]
                        + kernels[20] * 0.281621617903
                        + kernels[21]
                        + kernels[22]
                        + kernels[23]
                        + kernels[24] * 0.165377295408
                        + kernels[26]
                        + kernels[27]
                        + kernels[28]
                        + kernels[29]
                        + kernels[30]
                        + kernels[31]
                        + kernels[32]
                        + kernels[33]
                        + kernels[34]
                        + kernels[35]
                        + kernels[36] * 0.880524807473
                        + kernels[37] * 0.673866889617
                        + kernels[40] * -0.794186122061
                        - kernels[41]
                        + kernels[42] * -0.179236846339
                        - kernels[43]
                        - kernels[44]
                        - kernels[46]
                        - kernels[47]
                        - kernels[48]
                        + kernels[49] * -0.449320579982
                        - kernels[50]
                        - kernels[51]
                        - kernels[52]
                        - kernels[53]
                        - kernels[54]
                        - kernels[55]
                        - kernels[56]
                        - kernels[57]
                        - kernels[60]
                        - kernels[61]
                        - kernels[62]
                        - kernels[64]
                        - kernels[65]
                        - kernels[66]
                        - kernels[67]
                        + kernels[68] * -0.808837609759
                        + kernels[71] * -0.172080760708
                        - kernels[72]
                        - kernels[76]
                        - kernels[77]
                        - kernels[78]
                        + kernels[79] * -0.659760506313
                        - kernels[80]
                        ;
                        decisions[1] = -0.003195798296
                        + kernels[0] * 0.620973109333
                        + kernels[1] * 0.892303822786
                        + kernels[2] * 0.539869686696
                        + kernels[3] * 0.911376122356
                        + kernels[4]
                        + kernels[5]
                        + kernels[6]
                        + kernels[7]
                        + kernels[8]
                        + kernels[10]
                        + kernels[12]
                        + kernels[14]
                        + kernels[15]
                        + kernels[16]
                        + kernels[17]
                        + kernels[18]
                        + kernels[19]
                        + kernels[21]
                        + kernels[23]
                        + kernels[24] * 0.596393563799
                        + kernels[25] * 0.033061656163
                        + kernels[26]
                        + kernels[27]
                        + kernels[28]
                        + kernels[30]
                        + kernels[32]
                        + kernels[33]
                        + kernels[34]
                        + kernels[35]
                        + kernels[36] * 0.414341437909
                        + kernels[37] * 0.815874639529
                        + kernels[81] * -0.99658550696
                        - kernels[82]
                        + kernels[83] * -0.996929863393
                        - kernels[84]
                        - kernels[85]
                        - kernels[86]
                        - kernels[87]
                        - kernels[88]
                        - kernels[89]
                        - kernels[90]
                        + kernels[94] * -0.815427233321
                        - kernels[95]
                        - kernels[100]
                        - kernels[104]
                        - kernels[106]
                        - kernels[108]
                        + kernels[109] * -0.996666676387
                        - kernels[110]
                        - kernels[111]
                        - kernels[112]
                        - kernels[113]
                        - kernels[114]
                        + kernels[115] * -0.166867381855
                        - kernels[117]
                        + kernels[118] * -0.851717376654
                        - kernels[119]
                        - kernels[120]
                        - kernels[121]
                        - kernels[122]
                        ;
                        decisions[2] = 0.149224023965
                        + kernels[38]
                        + kernels[39]
                        + kernels[40]
                        + kernels[41] * 0.872054982675
                        + kernels[42]
                        + kernels[43]
                        + kernels[45]
                        + kernels[46]
                        + kernels[47]
                        + kernels[48] * 0.325935891762
                        + kernels[49] * 0.166486712911
                        + kernels[51]
                        + kernels[52]
                        + kernels[53]
                        + kernels[54]
                        + kernels[56]
                        + kernels[57]
                        + kernels[58]
                        + kernels[59]
                        + kernels[60]
                        + kernels[61]
                        + kernels[62]
                        + kernels[63]
                        + kernels[64]
                        + kernels[65]
                        + kernels[66]
                        + kernels[67]
                        + kernels[69] * 0.69438848288
                        + kernels[70]
                        + kernels[71]
                        + kernels[72]
                        + kernels[73]
                        + kernels[74]
                        + kernels[75]
                        + kernels[76]
                        + kernels[77]
                        + kernels[78]
                        + kernels[79] * 0.533468149429
                        - kernels[81]
                        - kernels[82]
                        - kernels[83]
                        - kernels[84]
                        - kernels[85]
                        - kernels[86]
                        - kernels[87]
                        - kernels[90]
                        - kernels[91]
                        - kernels[92]
                        - kernels[93]
                        - kernels[94]
                        - kernels[95]
                        - kernels[96]
                        - kernels[97]
                        - kernels[98]
                        - kernels[99]
                        - kernels[100]
                        - kernels[101]
                        - kernels[102]
                        + kernels[103] * -0.864784120542
                        - kernels[105]
                        + kernels[106] * -0.727550099115
                        - kernels[107]
                        - kernels[108]
                        - kernels[109]
                        - kernels[110]
                        - kernels[111]
                        - kernels[112]
                        - kernels[113]
                        - kernels[114]
                        - kernels[116]
                        - kernels[118]
                        - kernels[119]
                        - kernels[121]
                        - kernels[123]
                        ;
                        votes[decisions[0] > 0 ? 0 : 1] += 1;
                        votes[decisions[1] > 0 ? 0 : 2] += 1;
                        votes[decisions[2] > 0 ? 1 : 2] += 1;
                        int val = votes[0];
                        int idx = 0;

                        for (int i = 1; i < 3; i++) {
                            if (votes[i] > val) {
                                val = votes[i];
                                idx = i;
                            }
                        }

                        return idx;
                    }

                protected:
                    /**
                    * Compute kernel between feature vector and support vector.
                    * Kernel type: rbf
                    */
                    float compute_kernel(float *x, double value) {
                        //va_list w;
                        //va_start(w, 1);
                        //float kernel = 0.0;

                        //for (int i = 0; i < 1; i++) {
                        //    kernel += pow(x[i] - va_arg(w, double), 2);
                        //}

                        return exp(-0.01 * pow(x[0] - value, 2));
                    }
                };
            }
        }
    }