import React from 'react'
import { createNativeStackNavigator } from '@react-navigation/native-stack'

import { HomeScreen } from '../../features/home/screen'
import { UserDetailScreen } from '../../features/user/detail-screen'


// class vibinchairControl {
//   static socket = null;
//   static status = false;
//   constructor(){
//       this.init();
//   }
//   send = (message) => {
//       //maybe do a check to see if the socket is open & message would work
//       vibinchairControl.socket.send(JSON.stringify(message));
//   }
//   init = () => {
//       this.socket = new WebSocket('ws://vibinchair.local/ws');
//       this.status = false;
//       this.socket.onopen = () => {
//           this.status = true;
//           console.log("connected to server");
//       }
//       this.socket.onclose = () => {
//           this.status = false;
//           console.log("disconnected from server");
//       }
//       this.socket.onmessage = (message) => {
//           console.log(message);
//       }
//   }
// }


const Stack = createNativeStackNavigator<{
  home: undefined
  'user-detail': {
    id: string
  }
}>()

export function NativeNavigation() {
  const [vibinchairStatus, setVibinchairStatus] = React.useState(false);

  const vibinchair = React.useRef( new WebSocket('ws://vibinchair.local/ws'));
  console.log("vibinchair", vibinchair.current);
  vibinchair.current = new WebSocket('ws://vibinchair.local/ws');
  vibinchair.current.onopen = () => {
              console.log("connected to server");
              setVibinchairStatus(true);
          }

  vibinchair.current.onclose = () => {
              console.log("disconnected from server");
              setVibinchairStatus(false);
          }

  return (
    <Stack.Navigator>
      <Stack.Screen
        name="home"
        component={HomeScreen}
        options={{
          title: 'Home',
          headerStyle: {
            backgroundColor: vibinchairStatus ? '#095e05' : "#5e0506",
          },
          headerTintColor: '#fff',
        }}
      />
      <Stack.Screen
        name="user-detail"
        component={UserDetailScreen}
        options={{
          title: 'User',
        }}
      />
    </Stack.Navigator>
  )
}
