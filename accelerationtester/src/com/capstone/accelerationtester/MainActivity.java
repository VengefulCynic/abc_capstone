package com.capstone.accelerationtester;

import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.util.Log;

import java.io.*;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity { 
    private static final String TAG = MainActivity.class.getSimpleName() + " ";
    private Timer timer;
    private Boolean monitorRunning=false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView answerLabel = (TextView) findViewById(R.id.textView1);

        Button readacceleration = (Button) findViewById(R.id.readacceleration); 
        readacceleration.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate readacceleration onClick+");
                    
                log_acceleration();          
            }
        });

        Button setsamplerate = (Button) findViewById(R.id.setsamplerate); 
        setsamplerate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate setsamplerate+");
                
            }
        });

        Button startmonitor = (Button) findViewById(R.id.startmonitor); 
        startmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!monitorRunning)
                {
                    Log.i(TAG, "onCreate startmonitor+");

                    // Create the Handler object (on the main thread by default)
                    Handler handler = new Handler();

                    // Start the initial runnable task by posting through the handler
                    handler.post(runnableCode);

                    monitorRunning = true;
                }
                else
                {
                    Log.i(TAG, "startmonitor - the monitor is already running, knock it off!");                    
                }
                
            }
        });

        Button stopmonitor = (Button) findViewById(R.id.stopmonitor); 
        stopmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(monitorRunning)
                {
                    Log.i(TAG, "onCreate stopmonitor+");

                    // Removes pending code execution
                    handler.removeCallbacks(runnableCode);

                    monitorRunning = false;
                }
                else
                {
                    Log.i(TAG, "stopmonitor - the monitor isn't running yet, knock it off!");                    
                }
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
        //Get the text file
        File file = new File(path);

        //Read text from file
        StringBuilder text = new StringBuilder();

        try {
            BufferedReader br = new BufferedReader(new FileReader(file));
            String line;

            while ((line = br.readLine()) != null) {
                text.append(line);
                text.append('\n');
            }
            br.close();
        }
        catch (IOException e) {
            //You'll need to add proper error handling here
            Log.i(TAG, "Error "+ e);
        }
        return text.toString();
    }

// Define the code block to be executed
private Runnable runnableCode = new Runnable() {
    @Override
    public void run() {
      log_acceleration();
      // Repeat this the same runnable code block again another 2 seconds
      // 'this' is referencing the Runnable object
      handler.postDelayed(this, 500);
    }
};

private void log_acceleration()
{
    String x = readFile("/sys/bus/i2c/devices/1-0018/x_axis");
    String y = readFile("/sys/bus/i2c/devices/1-0018/y_axis");
    String z = readFile("/sys/bus/i2c/devices/1-0018/z_axis");
    Log.i(TAG, "onCreate readacceleration- x=" + x + ", y=" + y + ", z=" + z);ß
}

}
