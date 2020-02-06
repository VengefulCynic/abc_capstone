package com.capstone.accelerationtester;

//import android.os.AccelerometerManager; 
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity { 
    private static final String TAG = MainActivity.class.getSimpleName() + " ";
    private Timer timer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        AccelerometerManager manager = (AccelerometerManager) getSystemService ("AccelerometerManager");

        final TextView answerLabel = (TextView) findViewById(R.id.textView1);

        Button readacceleration = (Button) findViewById(R.id.readacceleration); 
        readacceleration.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate readacceleration onClick");
                int d = manager.readAcceleration();
                Log.i(TAG, "readAcceleration = " + d);
                //String answer = "Clicked Read Acceleration"; 
                //answerLabel.setText(answer);
            }
        });

        Button setsamplerate = (Button) findViewById(R.id.setsamplerate); 
        setsamplerate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate setsamplerate onClick");
                int d = manager.readAcceleration();
                Log.i(TAG, "setsamplerate = " + d);
                //String answer = "Clicked on Set Sample Rate"; 
                //answerLabel.setText(answer);
            }
        });

        Button startmonitor = (Button) findViewById(R.id.startmonitor); 
        startmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate startmonitor onClick");
                timer = new Timer(); 
                task = new TimerTask() 
                { 
                    public void run() { 
                        // do your work
                        Log.i(TAG, "onCreate startmonitor onClick task to schedule a readAccelerometer() each 20 seconds");
                        int d = manager.readAcceleration();
                        Log.i(TAG, "readAcceleration = " + d);
                        //String answer = "Clicked Read Acceleration";
                        //answerLabel.setText(answer);
                    } 
                }; 
                timer.schedule(task, 0, 60*(1000*20));// execute it each 20 seconds
                String answer = "Clicked on Start Monitor"; 
                answerLabel.setText(answer);
            }
        });

        Button stopmonitor = (Button) findViewById(R.id.stopmonitor); 
        stopmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate stopmonitor onClick");
                task.cancel();  // Stop the task
                timer.cancel();  //Terminates this timer,discarding any currently scheduled tasks.
                timer.purge();   // Removes all cancelled tasks from this timer's task queue.
                //String answer = "Clicked on Stop Monitor"; 
                //answerLabel.setText(answer);
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    public String readFile(String path) {
        File file = new File(path); 
        BufferedReader br = new BufferedReader(new FileReader(file)); 
        String st; 
        while ((st = br.readLine()) != null) 
            System.out.println(st); 
        } 
    }   
}
