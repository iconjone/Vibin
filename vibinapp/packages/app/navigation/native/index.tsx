import React from 'react'
import { createNativeStackNavigator } from '@react-navigation/native-stack'

import { HomeScreen } from '../../features/home/screen'
import { UserDetailScreen } from '../../features/user/detail-screen'
import Ionicons from '@expo/vector-icons/Ionicons'

import { vibinchairControl } from 'app/vibinConnection'

const Stack = createNativeStackNavigator<{
  home: undefined
  'user-detail': {
    id: string
  }
}>()

export function NativeNavigation() {
  const [vibinchairStatus, setVibinchairStatus] = React.useState(false);

  const vibinchair = React.useRef( new vibinchairControl());
  console.log("vibinchair", vibinchair.current);
  vibinchair.current = new vibinchairControl();
  vibinchair.current.socket.onopen = () => {
              console.log("connected to server");
              setVibinchairStatus(true);
          }

  vibinchair.current.socket.onclose = () => {
              console.log("disconnected from server");
              setVibinchairStatus(false);
              vibinchair.current.init();
          }

          vibinchair.current.socket.onerror = () => {
            console.log("disconnected from server");
            setVibinchairStatus(false);
            vibinchair.current.init();
        }


    

          // const char *type = doc["type"];     // "command"
          // const char *origin = doc["origin"]; // "device"
          // int nId = doc["nId"];               // 13000
          // const char *target = doc["target"]; // "device to change"

  return (
    <Stack.Navigator>
      <Stack.Screen
        name="home"
        options={{
          title: vibinchairStatus ? 'Connected 🪄' : "Connecting...📲",
          headerStyle: {
            backgroundColor: vibinchairStatus ? '#337530' : "#8f2728",
          },
          headerTintColor: '#fff',
          
        }}
      >
        {()=><HomeScreen vibinchair={vibinchair.current} vibinchairStatus />}
        </Stack.Screen>
      <Stack.Screen
        name="user-detail"
        component={UserDetailScreen}
        options={{
          title: 'DSP Settings',
        }}
      />
    </Stack.Navigator>
  )
}
