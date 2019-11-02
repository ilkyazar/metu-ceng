import java.util.ArrayList;

public class Session {

  private Track track;
  private ArrayList<Team> teamList;
  private int totalLaps;

  public Session() {
    // Fill this method
    this.track = null;
    this.teamList = null;
    this.totalLaps = 0;
  }

  public Session(Track track, ArrayList<Team> teamList, int totalLaps) {
    // Fill this method
    this.track = track;
    this.teamList = teamList;
    this.totalLaps = totalLaps;
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

  public void simulate() {
    // Fill this method
    if (this.track.isValidTrack()) {
      System.out.println("Track is valid.Strating simulation on " + this.track.getTrackName() + " for " + this.totalLaps + " laps.");
    }
    else {
      System.out.println("Track is invalid.Simulation aborted!");
      return;
    }
  }

  public String printWinnerTeam() {
    // Fill this method
    return ("printWinnerTeam");
  }

  private String printTimingTable() {
    // Fill this method
    return ("printTimingTable");
  }
}
