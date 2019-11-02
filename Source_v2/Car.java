public class Car {

  private int carNo;
  private String driverName;
  private double totalTime;
  private Tire tire;

  public Car() {
    // Fill this method
    this.carNo = 0;
    this.driverName = "";
    this.totalTime = 0;
    this.tire = null;
  }

  public Car(String driverName, int carNo, Tire tire) {
    // Fill this method
    this.carNo = carNo;
    this.driverName = driverName;
    this.totalTime = 0;
    this.tire = tire;
  }

  public Tire getTire() {
    return tire;
  }

  public void setTire(Tire tire) {
    this.tire = tire;
  }

  public String getDriverName() {
    return driverName;
  }

  public void setDriverName(String driverName) {
    this.driverName = driverName;
  }

  public int getCarNo() {
    return carNo;
  }

  public void setCarNo(int carNo) {
    this.carNo = carNo;
  }

  public double getTotalTime() {
    return totalTime;
  }

  public void tick(TrackFeature feature) {
    // Fill this method
  }

}
