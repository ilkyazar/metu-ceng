public class Monitor extends Part {
    private float size;
    private String aspectRatio;

    public Monitor() {
        super();
    }

    public float getSize() {
        return this.size;
    }

    public String getAspectRatio() {
        return this.aspectRatio;
    }

    public void setSize(float size) {
        this.size = size;
    }

    public void setAspectRatio(String aspectRatio) {
        this.aspectRatio = aspectRatio;
    }

    @Override
	public String toString(){
		return "\nBrand="+getBrand()+"\tModel="+getModel()+"\tPrice="+String.valueOf(getPrice())+"\tSize="+String.valueOf(getSize())+"\tAspectRatio="+getAspectRatio();
	}

}