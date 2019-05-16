
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;

import javax.imageio.ImageIO;

public class ConvertMain {
   public static void main(String args[]) throws Exception {
     String fileName = "test_picture.txt";
     ByteArrayToImage.convertToImage(new File(fileName), "JPG");
   }
}