import { Anchor, Button, H1, Paragraph, Separator, Sheet, XStack, YStack, Slider, Image } from '@my/ui'
import { ChevronDown, ChevronUp, Divide } from '@tamagui/lucide-icons'
import React, { useState } from 'react'
import { useLink } from 'solito/link'

export function HomeScreen( {vibinchair}) {
  const linkProps = useLink({
    href: '/user/nate',
  })
  console.log('HIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII');
  console.log("Homescreen", vibinchair);

  //function to send message to websocket
  const send = (message) => {
    //maybe do a check to see if the socket is open & message would work
    vibinchair.send(JSON.stringify(message));
  }


  
  return (
    <YStack f={1} jc="center" ai="center" p="$4" backgroundColor="#182030" space>
      <YStack space="$4" maw={600}>
        <H1 ta="center">Welcome to Vibin.</H1>
        <Paragraph ta="center">
          Connect to Vibin to get started.
        </Paragraph>
      </YStack>

      <XStack>
        <Button {...linkProps} theme="light">Link to user</Button>
        <Button onPress={() => send({type: "command", origin: "app", nId: Math.random(), target:"dsp", data:"nah" })}>Vibrate</Button>
      </XStack>

      <SheetDemo />
    </YStack>
  )
}

function SheetDemo() {
  const [open, setOpen] = useState(false)
  const [position, setPosition] = useState(0)
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
            <Slider size="$4" height={50} width={350} defaultValue={[50]} max={100} step={1} orientation="horizontal" theme="light" mx={25}>
                <Slider.Track>
                    <Slider.TrackActive />
                  </Slider.Track>
                <Slider.Thumb circular index={0} />
              </Slider>
              <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
              <Paragraph>TL</Paragraph>
               <Slider size="$4" height={50} width={150} defaultValue={[50]} max={100} step={1} orientation="horizontal" theme="green">
                <Slider.Track>
                    <Slider.TrackActive />
                  </Slider.Track>
                <Slider.Thumb circular index={0} />
              </Slider>
              <Slider size="$4" height={50} width={150} defaultValue={[50]} max={100} step={1} orientation="horizontal" theme="green">
                <Slider.Track>
                    <Slider.TrackActive />
                  </Slider.Track>
                <Slider.Thumb circular index={0} />
              </Slider>
              <Paragraph>TR</Paragraph>
              </XStack>
              <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
              <Paragraph>ML</Paragraph>
               <Slider size="$4" height={50} width={150} defaultValue={[50]} max={100} step={1} orientation="horizontal" theme="blue">
                <Slider.Track>
                    <Slider.TrackActive />
                  </Slider.Track>
                <Slider.Thumb circular index={0} />
              </Slider>
              <Slider size="$4" height={50} width={150} defaultValue={[50]} max={100} step={1} orientation="horizontal" theme="blue">
                <Slider.Track>
                    <Slider.TrackActive />
                  </Slider.Track>
                <Slider.Thumb circular index={0} />
              </Slider>
              <Paragraph>MR</Paragraph>
              </XStack>
              <XStack space als = "center"  bw={0} boc="$color" br="$4" p="$2" >
              <Paragraph>BF</Paragraph>
               <Slider size="$4" height={50} width={150} defaultValue={[50]} max={100} step={1} orientation="horizontal" theme="light_darker">
                <Slider.Track>
                    <Slider.TrackActive />
                  </Slider.Track>
                <Slider.Thumb circular index={0} />
              </Slider>
              <Slider size="$4" height={50} width={150} defaultValue={[50]} max={100} step={1} orientation="horizontal" theme="light_darker">
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
