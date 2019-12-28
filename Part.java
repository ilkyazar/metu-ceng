public class Part {
    private String brand;
    private String model;
    private float price;
    private String csvLine;
    private String type;

    public Part() {
        this.brand = null;
        this.model = null;
        this.price = 0;
    }

    public String getBrand() {
        return this.brand;
    }

    public String getModel() {
        return this.model;
    }

    public float getPrice() {
        return this.price;
    }

    public String getCsvLine() {
        return this.csvLine;
    }

    public String getType() {
        return this.type;
    }

    public void setBrand(String brand) {
        this.brand = brand;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public void setPrice(float price) {
        this.price = price;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setCsvLine(String line) {
        this.csvLine = line;
    }

}