import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in); 
  
        int n = in.nextInt(); 
        int m = in.nextInt(); 
        int numberOfUsers = in.nextInt(); 
  
        in.nextLine();

        for (int i = 0; i < numberOfUsers; i++) {
            String li = in.nextLine();

            String[] tokens = li.split("[ ]");

            String userName = tokens[0];
            System.out.println("name = " + userName);

            int numberOfSeats = tokens.length - 1;
            System.out.println("numberOfSeats = " + numberOfSeats);

            for (int j = 1; j < numberOfSeats + 1; j++)
                System.out.println(tokens[j]);

        }
    }
}