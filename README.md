# Image-Bitwise-Operations
Intersect two bitmaps using bitwise operations.

## A simple program to intersect two bitmaps by doing some bitwise operations.
We have three bitmaps:
## FirstBitmap.bmp
![FirstBitmap.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/FirstBitmap.bmp "FirstBitmap.bmp")
## SecondBitmap.bmp
![SecondBitmap.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/SecondBitmap.bmp "SecondBitmap.bmp")
## ThirdBitmap.bmp
![ThirdBitmap.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/ThirdBitmap.bmp "ThirdBitmap.bmp")
# The program flow is the following one:
## First step do the EXCLUSIVE OR (^) between the FirstBitmap.bmp and SecondBitmap.bmp and the result is:
### 1.XorBitmaps.bmp 
![1.XorBitmaps.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/1.XorBitmaps.bmp "1.XorBitmaps.bmp")
## Second step do the AND (&) between the 1.XorBitmaps.bmp and ThirdBitmap.bmp and the result is:
### 2.AndBitmaps.bmp
![2.AndBitmaps.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/2.AndBitmaps.bmp "2.AndBitmaps.bmp")
## Third step do the OR (|) between the 2.AndBitmaps.bmp and FirstBitmap.bmp and the result is:
### 3.OrBitmaps.bmp
![3.OrBitmaps.bmp](https://raw.githubusercontent.com/MateiEduardPetrisor/Image-Bitwise-Operations/master/Image%20Bitwise%20Operations/3.OrBitmaps.bmp "3.OrBitmaps.bmp")
