#
echo "===================== START ==================="
tunify '( P ( g _Vu ) _Vz ( f _Vz ) )' '( P _Vx _Vy ( f b ) )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( P _Vx ( f _Vy ) b )' '( P _Vx ( f b ) b )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( P _Vx )' '( P a )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( P ( f _Vx ) _Vy ( g _Vy ) )' '( P ( f _Vx ) _Vz ( g _Vx ) )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( P _Vx )' '( P ( f ( g B ) ) )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( P ( f _Vx ( g a _Vy ) ) ( g a _Vy ) )' '( P ( f _Vx _Vz ) _Vz )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( ~P ( f _Vx ( g a _Vy ) ) ( g a _Vy ) )' '( P ( f _Vx _Vz ) _Vz )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( P _Vx ( f _Vx ) a )' '( P _Vu _Vw _Vw )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( P _Vx ( f ( g _Vx ) ) a )' '( P b _Vy _Vz )'
echo "===================== END ==================="
#
echo "===================== START ==================="
tunify '( P _Vx ( f _Vx ) )' '( P ( f _Vy ) _Vy )'
echo "===================== END ==================="