import { Anchor, Button, H1, Paragraph, Separator, Sheet, XStack, YStack, Slider, Image } from '@my/ui'
import { ChevronDown, ChevronUp, Divide } from '@tamagui/lucide-icons'
import React, { useState } from 'react'
import { useLink } from 'solito/link'


export function SheetDemo(vibinchair) {
  vibinchair = vibinchair.vibinchair;
    const [open, setOpen] = useState(false)
    const [position, setPosition] = useState(0)
    console.log(vibinchair, "SHEET DEMO");
  
    const send = (message) => {
      //maybe do a check to see if the socket is open & message would work
      // console.log(vibinchair.vibinchair, "SEND");
      vibinchair.send(message);
      //heyyyyyy cutie IM IN YOUR CODE NOW
    }
  
    return (
      <>
        <Button
          size="$15"
          icon={open ? ChevronDown : ChevronUp}
          circular
          onPress={() => setOpen((x) => !x)}
          theme="light"
  
          
        />
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
              <Slider size="$4" height={50} width={350} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="light" mx={25}  onValueChange={(valuesend) => vibinchair.controlMasterVol(valuesend[0])}>
                 {/* onValueChange={(valuesend) => vibinchair.send(JSON.stringify({type: "command", origin: "app", nId: Math.random(), target:"amp1", data: JSON.stringify({command:"volume", value: valuesend})}))}> */}
                  <Slider.Track>
                      <Slider.TrackActive />
                    </Slider.Track>
                  <Slider.Thumb circular index={0} />
                </Slider>
                <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
                    <Paragraph>TL</Paragraph>
                    <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="green" onValueChange={(valuesend) => vibinchair.controlVol("amp1", "LM", valuesend[0])}>
                      <Slider.Track>
                          <Slider.TrackActive />
                        </Slider.Track>
                      <Slider.Thumb circular index={0} />
                    </Slider>
                    <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="green" onValueChange={(valuesend) => vibinchair.controlVol("amp1", "RM", valuesend[0])}>
                      <Slider.Track>
                          <Slider.TrackActive />
                        </Slider.Track>
                      <Slider.Thumb circular index={0} />
                    </Slider>
                    <Paragraph>TR</Paragraph>
                </XStack>
                <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
                    <Paragraph>ML</Paragraph>
                    <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="blue" onValueChange={(valuesend) => vibinchair.controlVol("amp2", "LM", valuesend[0])}>
                      <Slider.Track>
                          <Slider.TrackActive />
                        </Slider.Track>
                      <Slider.Thumb circular index={0} />
                    </Slider>
                    <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="blue" onValueChange={(valuesend) => vibinchair.controlVol("amp2", "RM", valuesend[0])}>
                      <Slider.Track>
                          <Slider.TrackActive />
                        </Slider.Track>
                      <Slider.Thumb circular index={0} />
                    </Slider>
                    <Paragraph>MR</Paragraph>
                </XStack>
                <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
                      <Paragraph>BF</Paragraph>
                      <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="light_darker" onValueChange={(valuesend) => vibinchair.controlVol("amp3", "LM", valuesend[0])}>
                        <Slider.Track>
                            <Slider.TrackActive />
                          </Slider.Track>
                        <Slider.Thumb circular index={0} />
                      </Slider>
                      <Slider size="$4" height={50} width={150} defaultValue={[1023]} max={1023} step={1} orientation="horizontal" theme="light_darker" onValueChange={(valuesend) => vibinchair.controlVol("amp3", "RM", valuesend[0])}>
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