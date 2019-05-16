import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;

import javax.imageio.ImageIO;

import org.apache.commons.io.FilenameUtils;

/*
 * Convert byte arrays into viewable images
 * Currently supported types:
 * 1. JPG images
 * 
 * Author: Haomin Yu
 */
public class ByteArrayToImage {
    
    /*
     * Converts the given 'data' into an image file of type
     * matching 'imageType'
     */
    public static void convertToImage(byte[] data, String imageType) throws IOException {
        imageType = imageType.toUpperCase();
        boolean successful = true;
        switch(imageType) {
            case "JPG":
                successful = convertToJPG(data);
                break;
            default:
                successful = false;
                System.out.println("'imageType' " + imageType + " currently not supported");
                break;
        }
        // Indicating whether the process was successful
        if(successful) {
            System.out.println(imageType + " image created successfully");
        }
        else {
            System.out.println("Image creation failed");
        }
    }
    
    /*
     * Converts the given bytes written in plain text in 'data' into an image file of type
     * matching 'imageType'
     * 
     * Throws IllegalArgumentException if 'data' is not a txt file
     */
    public static void convertToImage(File data, String imageType) throws IOException {
        String extension = FilenameUtils.getExtension(data.getAbsolutePath());
        if(!extension.equalsIgnoreCase("txt")) {
            throw new IllegalArgumentException("Expected txt file, but received " + extension + "file");
        }
        //convertToImage(Files.readAllBytes(data.toPath()), imageType);
    }
    
    /*
     * Converts the given bytes in 'data' into a JPG image file
     * Returns true if successful
     */
    private static boolean convertToJPG(byte[] data) throws IOException {
        ByteArrayInputStream stream = new ByteArrayInputStream(data);
        BufferedImage image = ImageIO.read(stream);
        int fileNumber  = 0;
        File outputFile = null;
        String fileName = null;
        do {
            fileName   = "output" + ++fileNumber + ".jpg";
            outputFile = new File(fileName);
        }
        while(outputFile.exists());
        ImageIO.write(image, "jpg", new File(fileName));
        return true;
    }
}
