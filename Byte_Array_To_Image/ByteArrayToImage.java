import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.List;

import org.apache.commons.codec.DecoderException;
import org.apache.commons.codec.binary.Hex;
import org.apache.commons.io.FileUtils;
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
    public static void convertToImage(File data, String imageType) throws IOException, DecoderException {
        String extension = FilenameUtils.getExtension(data.getAbsolutePath());
        if(!extension.equalsIgnoreCase("txt")) {
            throw new IllegalArgumentException("Expected txt file, but received " + extension + "file");
        }
        List<String> lineBytes = Files.readAllLines(data.toPath());
        // Figuring out the size of the byte array
        int lines = lineBytes.size();
        int firstLineBytes = lineBytes.get(0).length() / 2;
        int lastLineBytes = lineBytes.get(lines - 1).length() / 2;
        int byteArraySize = firstLineBytes * (lines - 1) + lastLineBytes;
        byte[] byteArray = new byte[byteArraySize];
        // Filling the 'byteArray'
        int nextIndex = 0;
        for(String lineByte: lineBytes) {
            byte[] decoded = Hex.decodeHex(lineByte);
            for(int i = 0; i < decoded.length; i++) {
                byteArray[nextIndex++] = decoded[i];
            }
        }
        // Converting to image
        convertToImage(byteArray, imageType);
    }
    
    /*
     * Converts the given bytes in 'data' into a JPG image file
     * Returns true if successful
     */
    private static boolean convertToJPG(byte[] data) throws IOException {
        int fileNumber  = 0;
        File outputFile = null;
        String fileName = null;
        do {
            fileName   = "output" + ++fileNumber + ".jpg";
            outputFile = new File(fileName);
        }
        while(outputFile.exists());
        FileUtils.writeByteArrayToFile(outputFile, data);
        return true;
    }
}
