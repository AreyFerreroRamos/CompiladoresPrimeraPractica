(3*5+(2.0-1))/2                                                                                                         # 8.00
var1 = div(8,4)                                                                                                         # var1 = 2
var2 = ""                                                                                                               # var2 = ""   
var3 = var2 * "Hola " * "mundo"                                                                                         # var3 = "Hola mundo"
length("Hola")                                                                                                          # 4
var4 = 5 *5 + (8+2)/4 + 6.5                                                                                             # var4 = 33.5
var5 =  (true && false) || 5>2                                                                                          # var5 = true
var5 = ( true && false) || 5<2                                                                                          # var5 = false
var2x3x4 = [[[0, 1, 2, 3]; [4, 5, 6, 7]; [8, 9, 10, 11]]; [[12, 13, 14.254, 15]; [16, 17, 18, 19]; [20, 21, 22, 23]]]   #var2x3x4 = [[[0.00, 1.00, 2.00, 3.00]; [4.00, 5.00, 6.00, 7.00]; [8.00, 9.00, 10.00, 11.00]]; [[12.00, 13.00, 14.25, 15.00]; [16.00, 17.00, 18.00, 19.00]; [20.00, 21.00, 22.00, 23.00]]]
length(var2x3x4)                                                                                                        # 24
size(var2x3x4)                                                                                                          # tmp =[2,3,4,]
var6 =3                                                                                                                 # var6 = 3
var6 = var2x3x4[0,2,var6]                                                                                               # var6 = 11.00
var6 = var2x3x4[1,0,0]                                                                                                  # var6 = 12.00
var6 = var2x3x4[1,2,3]                                                                                                  # var6 = 23.00
var2x3x4[0,0,0] = var6                                                                                                  # var2x3x4[0] = 23.00
var2x3x4[0,0,1] = 102                                                                                                   # var2x3x4[1] = 102
var2x3x4[0,0,2] = 103                                                                                                   # var2x3x4[2] = 103
var2x3x4[0,0,3] = 104                                                                                                   # var2x3x4[3] = 104
var = var2x3x4[0,0,3] + var2x3x4[0,0,3]                                                                                 # var = 208
var2x3x4                                                                                                                # var2x3x4 = [[[23.00, 102.00, 103.00, 104.00]; [4.00, 5.00, 6.00, 7.00]; [8.00, 9.00, 10.00, 11.00]]; [[12.00, 13.00, 14.25, 15.00]; [16.00, 17.00, 18.00, 19.00]; [20.00, 21.00, 22.00, 23.00]]]
varfinal = (var2x3x4 +var2x3x4) +( var2x3x4 +var2x3x4 -var2x3x4)                                                        # varfinal = [[[69.00, 306.00, 309.00, 312.00]; [12.00, 15.00, 18.00, 21.00]; [24.00, 27.00, 30.00, 33.00]]; [[36.00, 39.00, 42.75, 45.00]; [48.00, 51.00, 54.00, 57.00]; [60.00, 63.00, 66.00, 69.00]]]
multMatrix = 4 * var2x3x4 *4                                                                                            # multMatrix = [[[368.00, 1632.00, 1648.00, 1664.00]; [64.00, 80.00, 96.00, 112.00]; [128.00, 144.00, 160.00, 176.00]]; [[192.00, 208.00, 228.00, 240.00]; [256.00, 272.00, 288.00, 304.00]; [320.00, 336.00, 352.00, 368.00]]]
vector = [1,2]                                                                                                          # vector = [1, 2]
matrix4x2= [[0,1];[2,3];[4,5];[6,7]]                                                                                    # matrix4x2 = [[0, 1]; [2, 3]; [4, 5]; [6, 7]]
resulta = matrix4x2*vector                                                                                              # resulta = [2, 8, 14, 20]
vector = [0,1,2,3]                                                                                                      # vector = [0, 1, 2, 3]
resulta = vector* matrix4x2                                                                                             # resulta = [28, 34]
matrix2x4= [[0,1,2,3];[4,5,6,7]]                                                                                        # matrix2x4 = [[0, 1, 2, 3]; [4, 5, 6, 7]]
resulta = matrix2x4 *matrix4x2                                                                                          # resulta = [[28, 34]; [76, 98]]
resulta = matrix4x2 * matrix2x4 * matrix4x2                                                                             # resulta = [[76, 98]; [284, 362]; [492, 626]; [700, 890]]
matrix3x2 = [[2, 3]; [2, 3]; [2, 3]]                                                                                    # matrix3x2 = [[2, 3]; [2, 3]; [2, 3]]
size(matrix3x2)                                                                                                         # tmp =[3,2,]
matrix2x3 = transpose(matrix3x2)                                                                                        # matrix2x3 = [[2, 2, 2]; [3, 3, 3]]
size(matrix2x3)                                                                                                         # tmp[2,3]
m234 = zeros("Int32",2,3,4)                                                                                             # m234 = [[[0, 0, 0, 0]; [0, 0, 0, 0]; [0, 0, 0, 0]]; [[0, 0, 0, 0]; [0, 0, 0, 0]; [0, 0, 0, 0]]]
size(m234)                                                                                                              # tmp =[2,3,4]
length(m234)                                                                                                            # 24
m234 = ones("Float64",2,3,4)                                                                                            # m234 = [[[1.00, 1.00, 1.00, 1.00]; [1.00, 1.00, 1.00, 1.00]; [1.00, 1.00, 1.00, 1.00]]; [[1.00, 1.00, 1.00, 1.00]; [1.00, 1.00, 1.00, 1.00]; [1.00, 1.00, 1.00, 1.00]]]
size(m234)                                                                                                              # tmp =[2,3,4]
length(m234)                                                                                                            # 24
