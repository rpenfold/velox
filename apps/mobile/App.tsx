/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 */

import React from 'react';
import {
  SafeAreaView,
  ScrollView,
  StatusBar,
  StyleSheet,
  Text,
  View,
} from 'react-native';

function App(): React.JSX.Element {
  return (
    <SafeAreaView style={styles.container}>
      <StatusBar barStyle="dark-content" backgroundColor="#ffffff" />
      <ScrollView
        contentInsetAdjustmentBehavior="automatic"
        style={styles.scrollView}>
        <View style={styles.header}>
          <Text style={styles.title}>VeloxPlayground</Text>
          <Text style={styles.subtitle}>Formula Engine Testing App</Text>
        </View>
        <View style={styles.content}>
          <Text style={styles.description}>
            Welcome to VeloxPlayground! This app is designed for testing and
            experimenting with the Velox formula engine in a mobile environment.
          </Text>
          <View style={styles.featureList}>
            <Text style={styles.featureTitle}>Features:</Text>
            <Text style={styles.featureItem}>• Formula evaluation</Text>
            <Text style={styles.featureItem}>• Real-time testing</Text>
            <Text style={styles.featureItem}>• Performance monitoring</Text>
            <Text style={styles.featureItem}>• Cross-platform compatibility</Text>
          </View>
          <View style={styles.nativeInfo}>
            <Text style={styles.nativeTitle}>Native Module Ready</Text>
            <Text style={styles.nativeDescription}>
              This app is built with pure React Native CLI, allowing full access to
              native modules and custom C++ integration for the Velox formula engine.
            </Text>
          </View>
        </View>
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#ffffff',
  },
  scrollView: {
    flex: 1,
  },
  header: {
    padding: 20,
    alignItems: 'center',
    borderBottomWidth: 1,
    borderBottomColor: '#e0e0e0',
  },
  title: {
    fontSize: 28,
    fontWeight: 'bold',
    color: '#333333',
    marginBottom: 8,
  },
  subtitle: {
    fontSize: 16,
    color: '#666666',
    textAlign: 'center',
  },
  content: {
    padding: 20,
  },
  description: {
    fontSize: 16,
    lineHeight: 24,
    color: '#333333',
    marginBottom: 20,
    textAlign: 'center',
  },
  featureList: {
    backgroundColor: '#f8f9fa',
    padding: 16,
    borderRadius: 8,
    borderWidth: 1,
    borderColor: '#e9ecef',
    marginBottom: 20,
  },
  featureTitle: {
    fontSize: 18,
    fontWeight: '600',
    color: '#333333',
    marginBottom: 12,
  },
  featureItem: {
    fontSize: 14,
    color: '#555555',
    marginBottom: 8,
    paddingLeft: 8,
  },
  nativeInfo: {
    backgroundColor: '#e8f5e8',
    padding: 16,
    borderRadius: 8,
    borderWidth: 1,
    borderColor: '#c3e6c3',
  },
  nativeTitle: {
    fontSize: 16,
    fontWeight: '600',
    color: '#2d5a2d',
    marginBottom: 8,
  },
  nativeDescription: {
    fontSize: 14,
    color: '#4a7c4a',
    lineHeight: 20,
  },
});

export default App;
