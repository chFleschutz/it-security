
public class OneTimePad {
    /**
     * Liefert den String in Binärform als Byte-Array zurück.
     * Dabei wird jedes Symbol gemäß der erweiterten ASCII-Tabelle mit
     * 8 Bit als ein Byte codiert.
     * Diese Methode ist die "Umkehrfunktion"
     * zu {@link #getStringForm(String)}. 
     */
    public static byte[] getBinaryForm(String s) {
        // Bitte implementieren!
    }
    
    /**
     * Interpretiert das gegebene byte-Array als String.
     * Dafür wird jedes Byte als Symbol gemäß der erweiterten ASCII
     * Tabelle interpretiert. 
     * Diese Methode ist die "Umkehrfunktion"
     * zu {@link #getBinaryForm(String)}. 
     */
    public static String getStringForm(byte[] c) {
        // Bitte implementieren!
    }
    
    /**
     * Produziert ein (pseudo-)zufälliges Array von bytes mit der gegebenen Länge. 
     */
    public static byte[] getRandomKey(int length) {
        // Bitte implementieren!
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
        // Bitte implementieren!
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
        // Bitte implementieren!
    }
    
    /**
     * Gibt eine binäre Repräsentation des gegebenen byte-Arrays zurück.
     * Dabei wird jedes byte mit 8 Symbolen aus {0, 1} 
     * repräsentiert (auch die führenden 0-en!).
     */
    public static String toBinaryString(byte[] buf) {
        // Bitte implementieren!
    }
    
    public static void testBasics() {
        String klartext = "grüß die oma";
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
        System.out.println(getStringForm(decoded));
        System.out.println("Decodierter Klartext         : " + getStringForm(decoded));        
    }
    
    public static void main(String[] args) {
        testBasics();
    }
}
