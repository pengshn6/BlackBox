package top.niunaijun.blackbox.utils;

import android.location.Location;

import java.lang.reflect.Method;
import java.util.List;

import top.canyie.pine.Pine;
import top.canyie.pine.callback.MethodHook;
import top.niunaijun.blackbox.app.BActivityThread;
import top.niunaijun.blackbox.fake.frameworks.BLocationManager;

public class HackLocationUtils {

    public static final String TAG = "LocationUtils";

    public static void hackListener(Object listener){
        try {
            Method method = Reflector.findMethodByFirstName(listener.getClass(),"onLocationChanged");
            if(method != null){
                Pine.hook(method, new MethodHook() {
                    @Override public void beforeCall(Pine.CallFrame callFrame) {
                        if(callFrame.args[0] instanceof  List){
                            List<Location> locations = (List<Location>) callFrame.args[0];
                            locations.clear();
                            locations.add(BLocationManager.get().getLocation(BActivityThread.getUserId(), BActivityThread.getAppPackageName()).convert2SystemLocation());
                            callFrame.args[0] = locations;
                        }else if(callFrame.args[0] instanceof  Location){
                            callFrame.args[0] = BLocationManager.get().getLocation(BActivityThread.getUserId(), BActivityThread.getAppPackageName()).convert2SystemLocation();
                        }
                    }
                });
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
    }
}
