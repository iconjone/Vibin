import React from "react";
import {
    Text,
    Toggle,
    Button,

  } from "@ui-kitten/components";


  
  const Status = ({ vibinchair, serverState }) => {
    const [toggleState, setToggleState] = React.useState(false);
    const [toggleState1, setToggleState1] = React.useState(false);
    const [toggleState2, setToggleState2] = React.useState(false);
    const reconnect = () => {
      console.log("reconnecting", vibinchair);
      vibinchair.current.init();
    };
    const onCheckedChange = (isChecked) => {
        setToggleState(isChecked);
        if(isChecked)
         vibinchair.current.send({type: "command", origin: "app", nId:0, data: "en-on"});
           else
           vibinchair.current.send({type: "command", origin: "app", nId:0, data: "en-off"});
      };
        const onCheckedChange1 = (isChecked) => {
        setToggleState1(isChecked);
        if(isChecked)
            vibinchair.current.send({type: "command", origin: "app", nId:0, data: "m-on"});
              else
                vibinchair.current.send({type: "command", origin: "app", nId:0, data: "m-off"});
        };
        const onCheckedChange2 = (isChecked) => {
            setToggleState2(isChecked);
            if(isChecked)
                vibinchair.current.send({type: "command", origin: "app", nId:0, data: "r-on"});
                  else
                    vibinchair.current.send({type: "command", origin: "app", nId:0, data: "r-off"});
            };
    return (
      <>
        <Button
          category="h1"
          onPress={reconnect}
          status={serverState ? "success" : "danger"}
        >
          {serverState ? "Connected" : "Connecting..."}
        </Button>
        <Text category="h1"> Welcome to Vibin {vibinchair.status}</Text>
  
        <Toggle checked={toggleState} onChange={onCheckedChange} status='success'>
        Enable
      </Toggle>
      <Toggle checked={toggleState1} onChange={onCheckedChange1} status ='danger'>
        Mute
      </Toggle>
      <Toggle checked={toggleState2} onChange={onCheckedChange2} status='warning'>
        Reset
      </Toggle>
      </>
    );
  };

  export {Status};