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
    double time_spent = feature.getDistance()/this.tire.getSpeed() + Math.random();
    //System.out.println(time_spent);
    this.totalTime += time_spent;

    if (this.tire.getDegradation() > 70) {
      // Car is performing a pit stop

      Tire newTire;

      if (this.tire instanceof SoftTire) {
        newTire = new MediumTire();
      }
      else {
        newTire = new SoftTire();
      }

      this.tire = newTire;
      this.totalTime += 0.25;
    }
  }

}
