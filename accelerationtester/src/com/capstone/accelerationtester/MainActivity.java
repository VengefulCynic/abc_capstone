package com.capstone.accelerationtester;
 
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity { 

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
                String answer = "Clicked Read Acceleration"; 
                answerLabel.setText(answer);
            }
        });

        Button setsamplerate = (Button) findViewById(R.id.setsamplerate); 
        setsamplerate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub 
                String answer = "Clicked on Set Sample Rate"; 
                answerLabel.setText(answer);
            }
        });

        Button startmonitor = (Button) findViewById(R.id.startmonitor); 
        startmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub 
                String answer = "Clicked on Start Monitor"; 
                answerLabel.setText(answer);
            }
        });

        Button stopmonitor = (Button) findViewById(R.id.stopmonitor); 
        stopmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub 
                String answer = "Clicked on Stop Monitor"; 
                answerLabel.setText(answer);
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}
