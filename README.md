# Investment Tracking App

A small Qt Widgets desktop application for keeping track of investment and trading
positions by hand: what you bought, at what price, where your stop levels are, and
what that implies for risk, reward and running balance.

Written in 2023 as a personal tool and kept as-is.

## What it does

- Position table: instrument, buy date, buy cost, sell date, sell cost
- Stop-up and stop-loss levels per position
- Computed **risk/reward ratio**, absolute and relative profit per row
- Running balance derived from a starting bank
- Currency selector (USD, EUR, GBP)
- Sortable columns, add/remove rows
- Save and load from a tab-separated text file

## What it is not

This is a user interface exercise, not a trading system. It contains no market data,
no strategy, no signals and no order execution. It records positions that you type in
yourself and does arithmetic on them. Any real analysis lives elsewhere.

## Build

Requires a C++11 compiler and Qt Widgets (Qt 5 or Qt 6).

```sh
qmake && make          # Qt 5
qmake6 && make         # Qt 6
./InvestmentTrackingApp
```

Or open `InvestmentTrackingApp.pro` in Qt Creator.

There is no CMake build; the project uses qmake only.

## Layout

| File | Purpose |
|---|---|
| `main.cpp` | entry point |
| `mainwindow.{h,cpp,ui}` | main window, toolbar/menu actions, file open/save |
| `maintable.{h,cpp}` | `QTableWidget` subclass, row widgets, recalculation |

## Status

Unmaintained. Built against Qt 5 in 2023 and not continuously tested against newer
Qt releases. The UI uses fixed widget geometry rather than layouts, and old-style
`SIGNAL`/`SLOT` connections; if this were revived, both would be the first things
to change.

## License

GPL-3.0. See [LICENSE](LICENSE).
