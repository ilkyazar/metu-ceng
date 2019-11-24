import java.util.ArrayList;
import java.lang.*;

public class User implements Runnable {
    private String userName;
    private ArrayList<Seat> listOfWantedSeats;

    public User() {
        this.userName = "";
        this.listOfWantedSeats = new ArrayList<Seat>();
    }

    public User(String userName, ArrayList<Seat> listOfWantedSeats) {
        this.userName = userName;
        this.listOfWantedSeats = listOfWantedSeats;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getUserName() {
        return this.userName;
    }

    public void setlistOfWantedSeats(ArrayList<Seat> listOfWantedSeats) {
        this.listOfWantedSeats = listOfWantedSeats;
    }

    public ArrayList<Seat> getlistOfWantedSeats() {
        return this.listOfWantedSeats;
    }

    public void printUser() {
        System.out.println("Name of the user: " + this.getUserName());
        for (Seat seat: this.listOfWantedSeats) {
            System.out.println("Seat number: " + seat.getSeatNumber());
        }
    }

    @Override
    public void run() {
        System.out.println("Inside : " + Thread.currentThread().getName());
    }
}
