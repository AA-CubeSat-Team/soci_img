
import java.io.File;

public class ConvertMain {
   public static void main(String args[]) throws Exception {
     String fileName = "test1.txt";
     ByteArrayToImage.convertToImage(new File(fileName), "JPG");
   }
}