# Image-Bitwise-Operations
Intersect two bitmaps using bitwise operations.

## A simple program to intersect two bitmaps by doing some bitwise operations.
We have tree bitmaps:
- ![FirstBitmap.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/FirstBitmap.bmp)
- ![SecondBitmap.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/SecondBitmap.bmp)
- ![ThirdBitmap.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/ThirdBitmap.bmp)
The program flow is the following one:
- First do the Exclusive Or (XOR) between FirstBitmap.bmp and SecondBitmap.bmp and the result is stored in ![1.XorBitmaps.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/1.XorBitmaps.bmp)
- Second do the And (&) between the 1.XorBitmaps.bmp and ThirdBitmap.bmp and the result is ![2.AndBitmaps.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/2.AndBitmaps.bmp)
- Third do the Or(|) between 2.AndBitmaps.bmp and FirstBitmap.bmp and the result is ![3.OrBitmaps.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/3.OrBitmaps.bmp)
