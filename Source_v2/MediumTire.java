public class MediumTire extends Tire {

  public MediumTire() {
    this.speed = 310;
    this.degradation = 0;
    this.tireMultiplier = 1.1;
  }

  public void tick(TrackFeature f) {}
}
