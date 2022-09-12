import React from "react";
import {
    Text,
    Toggle,
    Button,

  } from "@ui-kitten/components";

const ToggleSimpleUsageShowcase = () => {
    const [checked, setChecked] = React.useState(false);
  
    const onCheckedChange = (isChecked) => {
      setChecked(isChecked);
    };
  
    return (
      <Toggle checked={checked} onChange={onCheckedChange}>
        {`Checked: ${checked}`}
      </Toggle>
    );
  };
  
  const Status = ({ vibinchair, serverState }) => {
    sendMessage = () => {
      console.log("sending message", vibinchair);
      vibinchair.current.send("test yeah");
    };
    return (
      <>
        <Button
          category="h1"
          onPress={sendMessage}
          status={serverState ? "success" : "danger"}
        >
          {serverState ? "Connected" : "Connecting..."}
        </Button>
        <Text category="h1"> Welcome to Vibin {vibinchair.status}</Text>
  
        <ToggleSimpleUsageShowcase />
      </>
    );
  };

  export {Status};