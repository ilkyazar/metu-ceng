public class GPU extends Part {
    private String chipset;
    private int capacity;
    private float clockSpeed;

    public GPU() {
        super();
    }

    public String getChipset() {
        return this.chipset;
    }

    public int getCapacity() {
        return this.capacity;
    }

    public float getClockSpeed() {
        return this.clockSpeed;
    }

    public void setChipset(String chipset) {
        this.chipset = chipset;
    }

    public void setClockSpeed(float clockSpeed) {
        this.clockSpeed = clockSpeed;
    }

    public void setCapacity(int capacity) {
        this.capacity = capacity;
    }

    @Override
	public String toString(){
		return "\nBrand="+getBrand()+"\tModel="+getModel()+"\tPrice="+String.valueOf(getPrice())+"\tChipset="+getChipset()+"\tCapacity="+String.valueOf(getCapacity())+"\tClockSpeed="+String.valueOf(getClockSpeed());
	}
}
