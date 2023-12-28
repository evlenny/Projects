#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Oct 10 2023
@author: Evan Lenahan
"""

# Importing libraries
import pandas as pd
import numpy as np
import yfinance as yf
import matplotlib.pyplot as plt

def format_currency(amount):
    return "${:,.2f}".format(amount)

def get_stock_data(ticker, start_date, end_date):
    data = yf.download(ticker, start=start_date, end=end_date)
    return data

def movAvg(data, window):
    return data['Close'].rolling(window=window).mean()

def movAvgCrossover(data, short_window, long_window, initial_capital):
    signals = pd.DataFrame(index=data.index)
    signals['signal'] = 0.0

    signals['short_mavg'] = movAvg(data, short_window)
    signals['long_mavg'] = movAvg(data, long_window)

    signals['signal'].iloc[short_window:] = np.where(signals['short_mavg'].iloc[short_window:] > signals['long_mavg'].iloc[short_window:], 1.0, 0.0)
    signals['positions'] = signals['signal'].diff()

    positions = pd.DataFrame(index=signals.index).fillna(0.0)
    positions['stock'] = 100 * signals['signal']

    portfolio = positions.multiply(data['Close'], axis=0)
    pos_diff = positions.diff()

    portfolio['holdings'] = (positions.multiply(data['Close'], axis=0)).sum(axis=1)
    portfolio['cash'] = initial_capital - (pos_diff.multiply(data['Close'], axis=0)).sum(axis=1).cumsum()
    portfolio['total'] = portfolio['cash'] + portfolio['holdings']
    portfolio['returns'] = portfolio['total'].pct_change()

    return signals, portfolio

def plot_signals(data, signals, portfolio, short_window, long_window):
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 12), gridspec_kw={'height_ratios': [3, 1]})

    ax1.plot(data.index, data['Close'], label='Close Price', alpha=0.5)
    ax1.plot(signals.index, signals['short_mavg'], label=f'Short Moving Average ({short_window} days)', alpha=0.5)
    ax1.plot(signals.index, signals['long_mavg'], label=f'Long Moving Average ({long_window} days)', alpha=0.5)

    buy_signals = signals[signals.positions == 1.0]
    sell_signals = signals[signals.positions == -1.0]

    ax1.plot(buy_signals.index, buy_signals.short_mavg, '^', markersize=10, color='g', label='Buy')
    ax1.plot(sell_signals.index, sell_signals.short_mavg, 'v', markersize=10, color='r', label='Sell')

    ax1.set_ylabel('Price')
    ax1.legend(loc='best')

    ax2.plot(portfolio.index, portfolio['total'], label='Portfolio Value')
    ax2.set_ylabel('Portfolio Value')
    ax2.legend(loc='best')

    plt.xlabel('Date')
    plt.show()

if __name__ == "__main__":
    print("This program will display buy and sell signals for a given stock based on a moving average crossover strategy.")
    print("A moving average crossover strategy involves tracking the short-term (short window) and long-term (long window) moving averages of a stock's price.")
    print("When the short-term moving average crosses above the long-term moving average, it generates a buy signal. Conversely, when it crosses below the long-term moving average, it generates a sell signal.\n")

    # User input
    ticker = input("Enter stock ticker symbol (e.g., AAPL for Apple Inc.): ")
    start_date = input("Enter start date (YYYY-MM-DD) for stock data: ")
    end_date = input("Enter end date (YYYY-MM-DD) for stock data: ")
    short_window = int(input("Enter short moving average window (e.g., 40 for a 40-day moving average): "))
    long_window = int(input("Enter long moving average window (e.g., 100 for a 100-day moving average, should be greater than short moving average window): "))
    initial_capital = float(input("Enter initial investment capital (e.g., 10000 for $10,000): "))

    # Analyzing data and generating signals
    data = get_stock_data(ticker, start_date, end_date)
    signals, portfolio = movAvgCrossover(data, short_window, long_window, initial_capital)
    
    # Displaying results
    print("\nGenerating a plot with buy and sell signals based on the moving average crossover strategy...")
    print("The top chart shows the stock's close price along with short and long moving averages.")
    print("Green markers (^) indicate buy signals, and red markers (v) indicate sell signals.")
    print("The bottom chart shows the portfolio value over time, assuming you follow the buy and sell signals.")
    print("This simulation starts with an initial investment capital, and it assumes buying 100 shares of the stock whenever a buy signal is generated, and selling all shares whenever a sell signal is generated.")
    
    # Plotting
    plot_signals(data, signals, portfolio, short_window, long_window)
    print("\nPlease note that this is a simple example and is not intended for use in real-world trading. Always do thorough research and testing before using any trading algorithm in a live environment.")
