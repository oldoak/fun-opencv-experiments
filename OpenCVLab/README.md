# ImageProcessing

**Task list [wiki]** for whenever I get back to this.  Last update December 2017.

## PixelIntensity

Experiments in altering pixel intensity of images. ```PixelIntensityBW.C ``` changes intensity in gray scale images. PixelIntensityBW.C takes two parameters, contrast and brightness, and adjusts these values in color images. The sample results show a high-lighted region using specific contrast and brightness values. Additionally, there has been some noise applied in the high-lighted region. The rest of the image outside the region has lowered intensity.

### Sample results
| **Original** | **Salt-and-Pepper Noise** |
|--------------|---------------------------|
|<img src="images/Superman_v_Hulk.jpg" height="150" width="200">|<img src="PixelIntensity/results/with_noise_pixel_intensity_color_sample.jpg" height="150" width="200">|

## FlatCoinDetection

Using Hough Transform to detect circular coins in an image, ```CoinDetection.C ```. The sample shows how the coin circumferences and centers are high-lighted. Additionally using pixel RBG brightness to high-light each quarter of the coins.

### Sample results
| **Original** | **Flat Coin detection** |
|--------------|-------------------------|
|<img src="images/Flat_and_Separate_Coins.jpg" height="100" width="200">|<img src="FlatCoinDetection/results/coins_detected.jpg" height="100" width="200">|

[wiki]: https://github.com/oldoak/ImageProcessing/wiki
