import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.Random;
 
public class Tester {
 
	public String displayTestCase(long seed) {
		//generateTestCase(seed);
		calculateExpected();
		String s = "Seed = " + seed + "\n\nCoins: " + coins + "\n";
		s += "Max Time: " + maxTime + "\n";
		s += "Note Time: " + noteTime + "\n";
		s += "Num Machines: " + no + "\n\n";
		for (int i = 0; i < no; i++) {
			s += "Machine " + i + "...\n";
			for (int j = 0; j < 3; j++)
				s += "Wheel " + j + ": " + wheels[i][j].substring(0, wheelSize[i]) + "\n";
			s += "Expected payout rate: " + expected[i] + "\n\n";
		}
		return s;
	}
 
	String[][] wheels;
	int coins;
	int maxTime;
	int noteTime;
	int no;
	Random[] machineState;
	int[] outs;
	double[] expected;
	int[] wheelSize;
	boolean failed = false;
 
	private int count(String s, char c) {
		int ret = 0;
		for (int i = 0; i < s.length(); i++) {
			if (s.charAt(i) == c)
				ret++;
		}
		return ret;
	}
 
	private void calculateExpected() {
		outs = new int[no];
		expected = new double[no];
		for (int i = 0; i < no; i++) {
			outs[i] += count(wheels[i][0], 'A') * count(wheels[i][1], 'A') * count(wheels[i][2], 'A') * 1000;
			outs[i] += count(wheels[i][0], 'B') * count(wheels[i][1], 'B') * count(wheels[i][2], 'B') * 200;
			outs[i] += count(wheels[i][0], 'C') * count(wheels[i][1], 'C') * count(wheels[i][2], 'C') * 100;
			outs[i] += count(wheels[i][0], 'D') * count(wheels[i][1], 'D') * count(wheels[i][2], 'D') * 50;
			outs[i] += count(wheels[i][0], 'E') * count(wheels[i][1], 'E') * count(wheels[i][2], 'E') * 20;
			outs[i] += count(wheels[i][0], 'F') * count(wheels[i][1], 'F') * count(wheels[i][2], 'F') * 10;
			outs[i] += count(wheels[i][0], 'G') * count(wheels[i][1], 'G') * count(wheels[i][2], 'G') * 5;
			expected[i] = 1.0 * outs[i] / wheels[i][0].length() / wheels[i][1].length() / wheels[i][2].length();
		}
	}
 
	private void generateTestCase(long seed) {
		Random r = new Random(seed);
		coins = 100 + r.nextInt(9901);
		maxTime = 100 + r.nextInt(9901);
		noteTime = 2 + r.nextInt(9);
		no = r.nextInt(8) + 3;
		machineState = new Random[no];
		wheels = new String[no][3];
		wheelSize = new int[no];
		for (int i = 0; i < no; i++) {
			machineState[i] = new Random(r.nextLong());
			if (seed == 0 && i == 0) {
				wheels[i][0] = wheels[i][1] = wheels[i][2] = "AABBBBCCCCCDDDDDDEEEEEEFFFFFFFGGGGGGGG";
				wheelSize[i] = wheels[i][0].length();
				continue;
			}
			wheelSize[i] = 10 + r.nextInt(21);
			for (int j = 0; j < 3; j++) {
				wheels[i][j] = "";
				for (int k = 0; k < wheelSize[i]; k++)
					wheels[i][j] += "AABBBBCCCCCDDDDDDEEEEEEFFFFFFFGGGGGGGG".charAt(r.nextInt(38));
				wheels[i][j] += wheels[i][j] + wheels[i][j];
			}
		}
		if (debug) {
			System.err.println(displayTestCase(seed));
		}
	}
 
	public int quickPlay(int machineNumber, int times) {
		if (times > maxTime) {
			System.err.println("Attempted to play after time ran out.\n");
			failed = true;
			return -1;
		}
		maxTime -= times;
		int ret = Integer.parseInt(doPlay(machineNumber, times)[0]);
		return ret;
	}
 
	public String[] notePlay(int machineNumber, int times) {
		if (times * noteTime > maxTime) {
			System.err.println("Attempted to play after time ran out.\n");
			failed = true;
			return new String[] { "-1" };
		}
		maxTime -= times * noteTime;
		String[] ret = doPlay(machineNumber, times);
		return ret;
	}
 
	public String[] doPlay(int machineNumber, int times) {
		if (failed)
			return new String[] { "-1" };
		int win = 0;
		String[] ret = new String[times + 1];
		for (int i = 0; i < times; i++) {
			if (coins <= 0) {
				System.err.println("Attempted to play after coins ran out.\n");
				failed = true;
				return new String[] { "-1" };
			}
			coins--;
			Random r = machineState[machineNumber];
			int w = wheelSize[machineNumber];
			int[] res = new int[] { r.nextInt(w), r.nextInt(w), r.nextInt(w) };
			ret[i + 1] = "";
			for (int j = -1; j <= 1; j++)
				for (int k = 0; k < 3; k++)
					ret[i + 1] += wheels[machineNumber][k].charAt(res[k] + w + j);
			if ((ret[i + 1].charAt(3) != ret[i + 1].charAt(4)) || (ret[i + 1].charAt(3) != ret[i + 1].charAt(5)))
				continue;
			if (ret[i + 1].charAt(3) == 'A')
				win += 1000;
			if (ret[i + 1].charAt(3) == 'B')
				win += 200;
			if (ret[i + 1].charAt(3) == 'C')
				win += 100;
			if (ret[i + 1].charAt(3) == 'D')
				win += 50;
			if (ret[i + 1].charAt(3) == 'E')
				win += 20;
			if (ret[i + 1].charAt(3) == 'F')
				win += 10;
			if (ret[i + 1].charAt(3) == 'G')
				win += 5;
		}
		ret[0] = "" + win;
		coins += win;
		return ret;
	}
 
	public void runTest(long seed) {
		Thread thread = null;
		if (exec != null) {
			try {
				Runtime rt = Runtime.getRuntime();
				proc = rt.exec(exec);
				os = proc.getOutputStream();
				is = proc.getInputStream();
				br = new BufferedReader(new InputStreamReader(is));
				thread = new ErrorReader(proc.getErrorStream());
				thread.start();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		generateTestCase(seed);
		playSlots(coins, maxTime, noteTime, no);
		if (failed) {
			System.err.println("FAIL");
			return;
		}
		System.err.println("seed=" + seed + ", score=" + coins);
		if (proc != null)
			try {
				proc.destroy();
			} catch (Exception e) {
				e.printStackTrace();
			}
		if (json) printJson(seed);
	}
 
	void playSlots(int coins, int maxTime, int noteTime, int num) {
		if (exec == null) {
			if (debug) {
				System.err.println("No exec");
			}
			return;
		}
		StringBuffer sb = new StringBuffer();
		sb.append(coins).append('\n');
		sb.append(maxTime).append('\n');
		sb.append(noteTime).append('\n');
		sb.append(num).append('\n');
 
		try {
			os.write(sb.toString().getBytes());
			os.flush();
 
			while (true) {
				String ret = br.readLine();
				sb = new StringBuffer();
				if (ret.equals("quick")) {
					int no = Integer.parseInt(br.readLine());
					int times = Integer.parseInt(br.readLine());
					int result = quickPlay(no, times);
					sb.append(result).append('\n');
				} else if (ret.equals("note")) {
					int no = Integer.parseInt(br.readLine());
					int times = Integer.parseInt(br.readLine());
					String[] result = notePlay(no, times);
					for (int i = 0; i < result.length; i++) {
						sb.append(result[i]).append('\n');
					}
				} else {
					break;
				}
				os.write(sb.toString().getBytes());
				os.flush();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return;
	}
 
	public static void main(String[] args) {
		String seed = "1";
		for (int i = 0; i < args.length; i++) {
			if (args[i].equals("-seed"))
				seed = args[++i];
			if (args[i].equals("-exec"))
				exec = args[++i];
			if (args[i].equals("-debug"))
				debug = true;
			if (args[i].equals("-json"))
				json = true;
		}
		Tester tester = new Tester();
		tester.runTest(Long.parseLong(seed));
	}
 
	public double[] score(double[][] raw) {
		int coderCnt = raw.length;
		if (coderCnt == 0)
			return new double[0];
		int testCnt = raw[0].length;
		double[] res = new double[raw.length];
		for (int test = 0; test < testCnt; test++) {
			double best = -1e100;
			for (int i = 0; i < coderCnt; i++)
				if (raw[i][test] >= -0.5) {
					best = Math.max(best, raw[i][test]);
				}
			if (best <= 0.001)
				continue;
			for (int i = 0; i < coderCnt; i++)
				if (raw[i][test] >= -0.5) {
					res[i] += raw[i][test] / best;
				}
		}
		for (int i = 0; i < res.length; i++) {
			res[i] /= testCnt;
			res[i] *= 1000000.0;
		}
		return res;
	}
 
	// ------------- visualization part ----------------------
	static Process proc;
	InputStream is;
	OutputStream os;
	BufferedReader br;
	static String exec;
	static boolean debug = false;
	static boolean json = false;
 
	public void printJson(long seed) {
		System.out.print("{");
		System.out.print("\"seed\":" + seed + ",");
		System.out.print("\"result\":" + coins + ",");
		System.out.print("\"resultTime\":" + maxTime + ",");
		generateTestCase(seed);
		calculateExpected();
		System.out.print("\"coins\":" + coins + ",");
		System.out.print("\"maxTime\":" + maxTime + ",");
		System.out.print("\"noteTime\":" + noteTime + ",");
		System.out.print("\"numMachines\":" + no + ",");
		System.out.print("\"machines\":[");
		for (int i = 0; i < no; i++) {
			if (i != 0) System.out.print(",");
			System.out.print("{");
			System.out.print("\"expected\":" + expected[i] + ",");
			System.out.print("\"wheelSize\":" + wheelSize[i] + ",");
			System.out.print("\"wheels\":[");
			for (int j = 0; j < 3; j++) {
				if (j != 0) System.out.print(",");
				System.out.print("\"" + wheels[i][j] + "\"");
			}
			System.out.print("]");
			System.out.print("}");
		}
		System.out.print("]");
		System.out.print("}");
		System.out.println("");
	}
 
}
 
class ErrorReader extends Thread{
	InputStream error;
	public ErrorReader(InputStream is) {
		error = is;
	}
	public void run() {
		try {
			byte[] ch = new byte[50000];
			int read;
			while ((read = error.read(ch)) > 0)
			{   String s = new String(ch,0,read);
				System.err.print(s);
				System.err.flush();
			}
		} catch(Exception e) { }
	}
}
