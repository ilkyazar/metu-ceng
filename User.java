import java.util.ArrayList;
import java.lang.*;

public class User implements Runnable {
    private String userName;
    private ArrayList listOfSeats;

    public User() {
        this.userName = "";
        this.listOfSeats = new ArrayList();
    }

    public User(String userName, ArrayList listOfSeats) {
        this.userName = userName;
        this.listOfSeats = listOfSeats;
    }

    public String getUserName() {
        return this.userName;
    }

    public ArrayList getListOfSeats() {
        return this.listOfSeats;
    }

    @Override
    public void run() {
        System.out.println("Inside : " + Thread.currentThread().getName());
    }
}
