import java.util.ArrayList;
import java.util.*;
import java.text.SimpleDateFormat;

public class Session {

  private Track track;
  private ArrayList<Team> teamList;
  private int totalLaps;

  // New instance variable
  private Team winnerTeam;

  public Session() {
    // Fill this method
    this.track = null;
    this.teamList = null;
    this.totalLaps = 0;
    this.winnerTeam = null;
  }

  public Session(Track track, ArrayList<Team> teamList, int totalLaps) {
    // Fill this method
    this.track = track;
    this.teamList = teamList;
    this.totalLaps = totalLaps;
    this.winnerTeam = null;
  }

  public Track getTrack() {
    return track;
  }

  public void setTrack(Track track) {
    this.track = track;
  }

  public ArrayList<Team> getTeamList() {
    return teamList;
  }

  public void setTeamList(ArrayList<Team> teamList) {
    this.teamList = teamList;
  }

  public int getTotalLaps() {
    return totalLaps;
  }

  public void setTotalLaps(int totalLaps) {
    this.totalLaps = totalLaps;
  }

  // New method
  public ArrayList<TrackFeature> getAllFeatures() {
    ArrayList<TrackFeature> features = new ArrayList<>();

    features.addAll(this.track.getFeatureList());

    return features;
  }

  // New method
  public void simulateLap() {
    ArrayList<TrackFeature> features = this.getAllFeatures();
    
    for (int f = 0; f < features.size(); f++) {
        for (int teamIndex = 0; teamIndex < this.teamList.size(); teamIndex++) {

            ArrayList<Car> cars = this.teamList.get(teamIndex).getCarList();

            for (int c = 0; c < cars.size(); c++) {
                
                cars.get(c).tick(features.get(f));
                cars.get(c).getTire().tick(features.get(f));

            }
        }
      }
  }

  public void simulate() {
    // Fill this method
    if (this.track.isValidTrack()) {
      System.out.println("Track is valid.Strating simulation on " + this.track.getTrackName() + " for " + this.totalLaps + " laps.");
      for (int lap = 0; lap < this.totalLaps; lap++) {
        simulateLap();
      }
      printWinnerTeam();
      printTimingTable();
    }
    else {
      System.out.println("Track is invalid.Simulation aborted!");
      return;
    }
  }

  public void printWinnerTeam() {
    // Fill this method
    double min_t = java.lang.Integer.MAX_VALUE;
    for (int teamIndex = 0; teamIndex < this.teamList.size(); teamIndex++) {
            ArrayList<Car> cars = this.teamList.get(teamIndex).getCarList();
            for (int c = 0; c < cars.size(); c++) {
                if (cars.get(c).getTotalTime() < min_t) {
                  this.winnerTeam = this.teamList.get(teamIndex);
                  min_t = cars.get(c).getTotalTime();
                }                
            }
        }
    String name = this.winnerTeam.getName();
    int n = this.winnerTeam.getTeamColors().length;
    String colors = new String();

    for (int i = 0; i < n; i++) {
      colors += this.winnerTeam.getTeamColors()[i];
      if (i == n - 2) {
        colors += " and ";
      }
      else if (i < n - 2) {
        colors += ", ";
      }
    }    
    System.out.println("Team " + name + " wins." + colors + " flags are waving everywhere.");

  }

  private void printTimingTable() {
    // Fill this method
    ArrayList<Car> cars = new ArrayList<>();
    String driverName, sdf;
    int carNo;

    for (int teamIndex = 0; teamIndex < this.teamList.size(); teamIndex++) {
      cars.addAll(this.teamList.get(teamIndex).getCarList());
    }

    Collections.sort(cars, Comparator.comparing(Car::getTotalTime));

    for (int c = 0; c < cars.size(); c++) {
      driverName = cars.get(c).getDriverName();
      carNo = cars.get(c).getCarNo();

      long millisecs = (long)(cars.get(c).getTotalTime() * 1000);
      Date date = new Date(millisecs);
      sdf = new SimpleDateFormat("HH:mm:ss.SSS").format(date);
      System.out.println(driverName + "(" + carNo + "): " + sdf);
    }
  }
}
