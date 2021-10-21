import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.*;

public class Seat {
    private Boolean taken;
    private String seatNumber;
    private User reservedUser;
    //private Lock seatLock = new ReentrantLock();
    private Semaphore sem = new Semaphore(1);

    public Seat() {
        this.taken = false;
        this.seatNumber = "";
        this.reservedUser = null;
    }

    public Seat(String seatNumber) {
        this.taken = false;
        this.seatNumber = seatNumber;
        this.reservedUser = null;
    }

    public void setSeatNumber(int r, int c) {
        String letter = Character.toString((char) r+65);
        String digit = Integer.toString(c);
        this.seatNumber = letter + digit;
    }

    public String getSeatNumber() {
        return this.seatNumber;
    }

    public void setTaken() {
        this.taken = true;
    }

    public Boolean isTaken() {
        return this.taken;
    }

    public void setEmpty() {
        this.taken = false;
        this.reservedUser = null;
    }

    public void setReservedUser(User user) {
        this.reservedUser = user;
    }

    public User getReservedUser() {
        return this.reservedUser;
    }

    public Semaphore getSem() {
        return this.sem;
    }

    public void reserve(User user) {
        try {
            sem.acquire();
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }

        try {
            doReserve(user);
        } finally {
            sem.release();
        }
    }

    public void doReserve(User user) {
        synchronized (this) {
            if (this.taken == false) {
                this.setTaken();
                this.setReservedUser(user);

                //System.out.println(this.getSeatNumber() + " is set as taken by " + getReservedUser().getUserName() + " " + System.nanoTime());
            }
        }
    }
}
