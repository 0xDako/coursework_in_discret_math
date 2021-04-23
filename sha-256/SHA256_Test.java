import javafx.util.Pair;

import javax.xml.bind.DatatypeConverter;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

enum Test {
    SIMPLE,
    EMPTY,
    LONG;
}

public class SHA256_Test {
    public static Map<Test, Pair<String, String>> rightHash;

    public static void main(String[] args) {
        fillTestHash();
        testHW(Test.SIMPLE);
        testHW(Test.LONG);
        testHW(Test.EMPTY);
    }

    static void testHW(Test chosen) {
        byte[] test = rightHash.get(chosen).getKey().getBytes(StandardCharsets.US_ASCII);;
        byte[] myHash = SHA256.hash(test);
        String myHashStr = DatatypeConverter.printHexBinary(myHash).toLowerCase();
        System.out.println(myHashStr);
        String right = rightHash.get(chosen).getValue();
        System.out.println(myHashStr.equals(right));
    }

    static void fillTestHash() {
        rightHash = new HashMap<>();
        rightHash.put(Test.SIMPLE, new Pair<>("Hello world!", "c0535e4be2b79ffd93291305436bf889314e4a3faec05ecffcbb7df31ad9e51a"));
        rightHash.put(Test.EMPTY, new Pair<>("", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
        rightHash.put(Test.LONG, new Pair<>("Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                + "Proin pulvinar turpis purus, sit amet dapibus magna commodo "
                + "quis metus.", "60497604d2f6b4df42cea5efb8956f587f81a4ad66fa1b65d9e085224d255036"));
    }
}
