import { Anchor, Button, H1, Paragraph, Separator, Sheet, XStack, YStack, Slider, Image } from '@my/ui'
import { ChevronDown, ChevronUp, Divide } from '@tamagui/lucide-icons'
import React, { useCallback, useState } from 'react'
import {Vibration} from 'react-native'
import { useLink } from 'solito/link'

import throttle from 'lodash.throttle'



export function SheetDemo(vibinchair) {
  vibinchair = vibinchair.vibinchair;
    const [open, setOpen] = useState(false)
    const [position, setPosition] = useState(0)
    // console.log(vibinchair, "SHEET DEMO");

    const [masterControl, setMasterControl] = useState(0);
  
    const send = (message) => {
      //maybe do a check to see if the socket is open & message would work
      // console.log(vibinchair.vibinchair, "SEND");
      vibinchair.send(message);
      //heyyyyyy cutie IM IN YOUR CODE NOW
    }

    //throttle the send function
    // const throttledSend = useCallback(
    //   throttle(send, 1000, {trailing: false}),
    //   []
    // );

    const throttledSend = throttle(send, 1020, {trailing: false});



    const masterThrottle = useCallback(throttle(vibinchair.controlMasterVol, 700, {trailing: true}), []);

    const volumeThrottle = useCallback(throttle(vibinchair.controlVol, 700, {trailing: true}), []);


  
    return (
      <>
        <Button
          size="$8"
          icon={open ? ChevronDown : ChevronUp}
          iconAfter={open ? ChevronDown : ChevronUp}
          theme="dark_SliderTrack"
          onPress={() => setOpen((x) => !x)}
          onSwipeUp={() => setOpen((x) => !x)}
          onSwipeDown={() => setOpen((x) => !x)}
          onSwipeLeft={() => setOpen((x) => !x)}
  
          
        >
          Levels
          </Button>
        <Sheet
          modal
          open={open}
          onOpenChange={setOpen}
          snapPoints={[95]}
          position={position}
          onPositionChange={setPosition}
          dismissOnSnapToBottom
        >
          <Sheet.Overlay />
          <Sheet.Frame ai="center" jc="center">
            <Sheet.Handle />
            <Paragraph ta="center">
           Set your levels for the chair below.
          </Paragraph>
          <Image src={require('./chair.png')} width={200} height={300} />
          <Separator my={5} theme="dark"/>
  
  <XStack space als = "center">
  <YStack flex={1} space="$2"  bw={0} boc="$color" br="$4" p="$2" als = "center">
          <Paragraph>Master</Paragraph>
              <Slider size="$4" height={50} width={350} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="light" mx={25}  onValueChange={(valuesend) => {masterThrottle(valuesend[0]); Vibration.vibrate();}}> 
                  <Slider.Track>
                      <Slider.TrackActive />
                    </Slider.Track>
                  <Slider.Thumb circular index={0} />
                </Slider>
                <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
                    <Paragraph>TL</Paragraph>
                    <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="green" onValueChange={(valuesend) => {volumeThrottle("amp1", "LM", valuesend[0]); Vibration.vibrate();}}>
                      <Slider.Track>
                          <Slider.TrackActive />
                        </Slider.Track>
                      <Slider.Thumb circular index={0} />
                    </Slider>
                    <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="green" onValueChange={(valuesend) => {volumeThrottle("amp1", "RM", valuesend[0]); Vibration.vibrate();}}>
                      <Slider.Track>
                          <Slider.TrackActive />
                        </Slider.Track>
                      <Slider.Thumb circular index={0} />
                    </Slider>
                    <Paragraph>TR</Paragraph>
                </XStack>
                <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
                    <Paragraph>ML</Paragraph>
                    <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="blue" onValueChange={(valuesend) => {volumeThrottle("amp2", "LM", valuesend[0]); Vibration.vibrate();}}>
                      <Slider.Track>
                          <Slider.TrackActive />
                        </Slider.Track>
                      <Slider.Thumb circular index={0} />
                    </Slider>
                    <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="blue" onValueChange={(valuesend) => {volumeThrottle("amp2", "RM", valuesend[0]); Vibration.vibrate();}}>
                      <Slider.Track>
                          <Slider.TrackActive />
                        </Slider.Track>
                      <Slider.Thumb circular index={0} />
                    </Slider>
                    <Paragraph>MR</Paragraph>
                </XStack>
                <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
                      <Paragraph>BF</Paragraph>
                      <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="light_darker" onValueChange={(valuesend) => {volumeThrottle("amp3", "LM", valuesend[0]); Vibration.vibrate();}}>
                        <Slider.Track>
                            <Slider.TrackActive />
                          </Slider.Track>
                        <Slider.Thumb circular index={0} />
                      </Slider>
                      <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="light_darker" onValueChange={(valuesend) => {volumeThrottle("amp3", "RM", valuesend[0]); Vibration.vibrate();}}>
                        <Slider.Track>
                            <Slider.TrackActive />
                          </Slider.Track>
                        <Slider.Thumb circular index={0} />
                      </Slider>
                      <Paragraph>BB</Paragraph>
                </XStack>
  
    </YStack>
  </XStack>
          
            <Button
              size="$6"
              circular
              icon={ChevronDown}
              onPress={() => {
                setOpen(false)
              }}
            />
          </Sheet.Frame>
        </Sheet>
      </>
    )
  }