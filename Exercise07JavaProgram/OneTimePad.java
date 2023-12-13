import java.security.SecureRandom;

public class OneTimePad {
    /**
     * Liefert den String in Binärform als Byte-Array zurück.
     * Dabei wird jedes Symbol gemäß der erweiterten ASCII-Tabelle mit
     * 8 Bit als ein Byte codiert.
     * Diese Methode ist die "Umkehrfunktion"
     * zu {@link #getStringForm(String)}. 
     */
    public static byte[] getBinaryForm(String s) {
        return s.getBytes();
    }
    
    /**
     * Interpretiert das gegebene byte-Array als String.
     * Dafür wird jedes Byte als Symbol gemäß der erweiterten ASCII
     * Tabelle interpretiert. 
     * Diese Methode ist die "Umkehrfunktion"
     * zu {@link #getBinaryForm(String)}. 
     */
    public static String getStringForm(byte[] c) {
        return new String(c);
    }
    
    /**
     * Produziert ein (pseudo-)zufälliges Array von bytes mit der gegebenen Länge. 
     */
    public static byte[] getRandomKey(int length) {
        // Create a SecureRandom instance for secure random number generation
        SecureRandom secureRandom = new SecureRandom();

        // Generate a random byte array of the specified length
        byte[] randomKey = new byte[length];
        secureRandom.nextBytes(randomKey);

        return randomKey;
    }
    
    /**
     * Verschlüsselt die gegebene Nachricht msg mit dem
     * gegebenen Schlüssel key gemäß dem Vorgehen von Vernams 
     * One-Time-Pad. 
     * Wirft eine IllegalArgumentException, falls eine der
     * Eingaben null ist oder falls die Länge der Arrays nicht 
     * übereinstimmen. 
     */
    public static byte[] encode(byte[] msg, byte[] key) {
        byte[] encodedMessage = new byte[msg.length];
        
        for (int i = 0; i < msg.length; i++) {
            encodedMessage[i] = (byte) (msg[i] ^ key[i]);
        }

        return encodedMessage;
    }
    
    /**
     * Entschlüsselt den gegebenen Chiffretext chiffre mit dem
     * gegebenen Schlüssel key gemäß dem Vorgehen von Vernams 
     * One-Time-Pad. 
     * Wirft eine IllegalArgumentException, falls eine der
     * Eingaben null ist oder falls die Länge der Arrays nicht 
     * übereinstimmen. 
     */
    public static byte[] decode(byte[] chiffre, byte[] key) {
        return encode(chiffre, key);
    }
    
    /**
     * Gibt eine binäre Repräsentation des gegebenen byte-Arrays zurück.
     * Dabei wird jedes byte mit 8 Symbolen aus {0, 1} 
     * repräsentiert (auch die führenden 0-en!).
     */
    public static String toBinaryString(byte[] buf) {
        StringBuilder binaryStringBuilder = new StringBuilder();

        for (byte b : buf) {
            // Convert each byte to its binary representation
            String binaryString = String.format("%8s", Integer.toBinaryString(b & 0xFF)).replace(' ', '0');
            
            // Append the binary representation to the StringBuilder
            binaryStringBuilder.append(binaryString);
        }

        return binaryStringBuilder.toString();
    }
    
    public static void testBasics() {
        String klartext = "helloworld";

        byte[] m = getBinaryForm(klartext);
        System.out.println("Klartext                     : " + klartext);
        System.out.println("Klartext (binär)             : " + toBinaryString(m));
        byte[] k = getRandomKey(m.length);
        System.out.println("Schlüssel                    : " + toBinaryString(k));

        byte[] c = encode(m, k);
        System.out.println("Chiffretext (binär)          : " + toBinaryString(c));
        System.out.println("Chiffretext                  : " + getStringForm(c));

        byte[] decoded = decode(c, k);
        System.out.println("Decodierter Klartext (binär) : " + toBinaryString(decoded));
        System.out.println("Decodierter Klartext         : " + getStringForm(decoded));        
    }
    
    public static void main(String[] args) {
        testBasics();
    }
}
