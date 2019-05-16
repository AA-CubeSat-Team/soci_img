
import java.io.File;

public class ConvertMain {
   public static void main(String args[]) throws Exception {
     String fileName = "test_picture.txt";
     ByteArrayToImage.convertToImage(new File(fileName), "JPG");
   }
}