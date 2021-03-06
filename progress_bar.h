/*******************************************************************************
 * progress_bar.h
 *
 * Progress Bar utility
 *
 *******************************************************************************
 * Copyright (C) 2016 Lorenz Hübschle-Schneider <lorenz@4z2.de>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/


#pragma once

#include <iostream>

/// A simple progress bar
class progress_bar {
public:
    /// Create a new progress bar
    /// \param max the value that constitutes 100%
    /// \param out the output stream to draw the progress bar on
    /// \param barwidth the width of the bar in characters
    progress_bar(const unsigned long long max, const std::string &extra,
                 std::ostream &out = std::cout, int barwidth = 70)
        : out(out)
        , extra(extra)
        , max(max)
        , pos(0)
        , lastprogress(-1)
        , barwidth(barwidth)
        , do_draw((out.rdbuf() == std::cout.rdbuf() || out.rdbuf() == std::cerr.rdbuf())) {}

    /// increase progress by 1 step (not percent!)
    void step() {
        ++pos;
        draw();
    }

    /// set progress to a position
    /// \param newpos position to set the progress to (steps, not percent!)
    void stepto(unsigned long long newpos) {
        pos = newpos;
        draw();
    }

    void operator++() {
        step();
    }

    /// remove all traces of the bar from the output stream
    void undraw() {
        out << "\r";
        // "[" + "] " + percent (3) + " %" = up to 8 chars
        int width = barwidth + 8 + static_cast<int>(extra.length());
        for (int i = 0; i < width; ++i) {
            out << " ";
        }
        out << "\r";
    }

    void set_extra(const std::string &new_extra) {
        undraw();
        extra = new_extra;
        draw();
    }

protected:
    // adapted from StackOverflow user "leemes":
    // http://stackoverflow.com/a/14539953
    /// Draw the progress bar to the output stream
    void draw() {
        if (!do_draw) return;
        int progress = static_cast<int>((pos * 100) / max);
        if (progress == lastprogress) return;

        out << extra << "[";
        int pos = barwidth * progress / 100;
        for (int i = 0; i < barwidth; ++i) {
            if (i < pos)
                out << "=";
            else if (i == pos)
                out << ">";
            else
                out << " ";
        }
        out << "] " << progress << " %\r";
        out.flush();

        lastprogress = progress;
    }

private:
    std::ostream &out;
    std::string extra;
    unsigned long long max;
    unsigned long long pos;
    int lastprogress;
    const int barwidth;
    const bool do_draw;
};
