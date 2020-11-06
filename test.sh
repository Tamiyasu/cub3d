#!/bin/sh -x
echo "test 1 : without argcs"
./cub3D
echo "test 2 : with argcs without correct extention."
./cub3D test
echo "test 3 : with argcs with correct extention (not exist file)."
./cub3D test.cub
echo "test 4 : with argcs with dummy extention."
./cub3D test.cub.test
echo "test 5 : with argcs with dummy extention and correct extention "
./cub3D test.cub.test.cub
echo "test 6 : correct filename / incorrect filecontent "
./cub3D ./cubfiles/test.cub
echo "test 7 : correct filename / filecontent with 3rd arg."
./cub3D ./cubfiles/test2.cub --save --save
echo "test 8 : correct filename / filecontent with wrong 2rd arg (--save--save)."
./cub3D ./cubfiles/test2.cub --save--save
echo "test 9 : correct filename / filecontent with wrong 2rd arg (---save)."
./cub3D ./cubfiles/test2.cub ---save
echo "test 10 : correct filename / filecontent with wrong 2rd arg (-save)."
./cub3D ./cubfiles/test2.cub -save
echo "test e : with error cub files."
echo "test e1 : R 1000a 1600 (inacceptable char at x)"
./cub3D ./cubfiles/test_error1.cub
echo "test e2 : R 1000 1600b (inacceptable char at y)"
./cub3D ./cubfiles/test_error2.cub
echo "test e3 : R 0 0 (both of x, y is 0)"
./cub3D ./cubfiles/test_error3.cub
echo "test e4 : R 10 0 (y is 0)"
./cub3D ./cubfiles/test_error4.cub
echo "test e5 : R 0 10 (x is 0)"
./cub3D ./cubfiles/test_error5.cub
echo "test e6 : R is twice"
./cub3D ./cubfiles/test_error6.cub
echo "test e7 : Non_exists_xpm"
./cub3D ./cubfiles/test_error7.cub
echo "test e8 : map twice"
./cub3D ./cubfiles/test_error8.cub
echo "test e9 : some extracts after map"
./cub3D ./cubfiles/test_error9.cub
echo "test e10 : map is not closed1 simple case"
./cub3D ./cubfiles/test_error10.cub
echo "test e11 : map is not closed1 maze case"
./cub3D ./cubfiles/test_error11.cub
echo ""
echo "test 7 : Correct case."
./cub3D ./cubfiles/test2.cub
