import React from "react";
import { LineChart } from "react-native-chart-kit";
import { Text } from "@ui-kitten/components";
import { Dimensions } from "react-native";
const HeartRate = ({ heartRate }) => {
    return(
  <>
    <Text category="h5" alignment="center" >Live BPM: {heartRate[heartRate.length - 1]}</Text>
    <LineChart
      data={{
        datasets: [
          {
            data: heartRate,
          },
        ],
      }}
      width={Dimensions.get("window").width - 20} // from react-native
      height={256}
      verticalLabelRotation={30}
      chartConfig={{
        backgroundColor: "#e26a00",
        backgroundGradientFrom: "#fb8c00",
        backgroundGradientTo: "#ffa726",
        decimalPlaces: 2, // optional, defaults to 2dp
        color: (opacity = 1) => `rgba(255, 255, 255, ${opacity})`,
        labelColor: (opacity = 1) => `rgba(255, 255, 255, ${opacity})`,
        style: {
          borderRadius: 16,
        },
        propsForDots: {
          r: "6",
          strokeWidth: "2",
          stroke: "#ffa726",
        },
      }}
      bezier
      style={{
        marginVertical: 8,
        borderRadius: 16,
      }}
    />
  </>
    )
};
export {HeartRate};