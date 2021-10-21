public class Memory extends Part {
    private String socket;
    private float clockSpeed;
    private int capacity;

    public Memory() {
        super();
    }

    public String getSocket() {
        return this.socket;
    }

    public float getClockSpeed() {
        return this.clockSpeed;
    }

    public int getCapacity() {
        return this.capacity;
    }

    public void setCapacity(int capacity) {
        this.capacity = capacity;
    }

    public void setSocket(String socket) {
        this.socket = socket;
    }

    public void setClockSpeed(float clockSpeed) {
        this.clockSpeed = clockSpeed;
    }

    @Override
	public String toString(){
		return "\nBrand="+getBrand()+"\tModel="+getModel()+"\tPrice="+String.valueOf(getPrice())+"\tSocket="+getSocket()+"\tCapacity="+String.valueOf(getCapacity())+"\tClockSpeed="+String.valueOf(getClockSpeed());
	}

}
