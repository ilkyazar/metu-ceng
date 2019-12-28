public class CPU extends Part {
    private int coreCount;
    private float clockSpeed;

    public CPU() {
        super();
    }

    public int getCoreCount() {
        return this.coreCount;
    }

    public float getClockSpeed() {
        return this.clockSpeed;
    }

    public void setCoreCount(int coreCount) {
        this.coreCount = coreCount;
    }

    public void setClockSpeed(float clockSpeed) {
        this.clockSpeed = clockSpeed;
    }

    @Override
	public String toString(){
		return "\nBrand="+getBrand()+"\tModel=="+getModel()+"\tPrice="+String.valueOf(getPrice())+"\tCoreCount="+String.valueOf(getCoreCount())+"\tClockSpeed="+String.valueOf(getClockSpeed());
	}
}
