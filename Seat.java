public class Seat {
    private Boolean taken;
    private String seatNumber;
    private User reservedUser;

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

    public void setReservedUser(User user) {
        this.reservedUser = user;
    }

    public User getReservedUser() {
        return this.reservedUser;
    }
}
