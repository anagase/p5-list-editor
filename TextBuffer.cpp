#include "TextBuffer.hpp"

// Constructor
TextBuffer::TextBuffer() {
    cursor = data.end();
    row = 1;
    column = 0;
    index = 0;
}

bool TextBuffer::forward() {
    if (cursor == data.end()) {
        return false;
    }
    
    char current_char = *cursor;
    ++cursor;
    ++index;
    
    if (current_char == '\n') {
        ++row;
        column = 0;
    } else {
        ++column;
    }
    
    return true;
}

bool TextBuffer::backward() {
    if (cursor == data.begin()) {
        return false;
    }
    
    --cursor;
    --index;
    
    if (*cursor == '\n') {
        --row;
        column = compute_column();
    } else {
        --column;
    }
    
    return true;
}

void TextBuffer::insert(char c) {
    data.insert(cursor, c);
    // cursor remains pointing to the same element (after insertion)
    
    if (c == '\n') {
        ++row;
        column = 0;
    } else {
        ++column;
    }
    ++index;
}

bool TextBuffer::remove() {
    if (cursor == data.end()) {
        return false;
    }
    
    char removed_char = *cursor;
    cursor = data.erase(cursor);
    
    if (removed_char == '\n') {
        // Removing a newline merges the next row into current row
        // We stay at the same row, but need to recalculate column
        column = compute_column();
    }
    // For non-newline characters, row and column stay the same
    // (cursor now points to what was the next character)
    
    // Update index to match cursor position
    if (cursor == data.end()) {
        index = static_cast<int>(data.size());
    }
    
    return true;
}

void TextBuffer::move_to_row_start() {
    // Move backward to find the newline that starts this row
    while (cursor != data.begin()) {
        auto prev = cursor;
        --prev;
        if (*prev == '\n') {
            break;
        }
        --cursor;
        --index;
    }
    column = 0;  // At start of row
}

void TextBuffer::move_to_row_end() {
    // Move forward to find the newline that ends this row
    while (cursor != data.end() && *cursor != '\n') {
        ++cursor;
        ++index;
    }
    column = compute_column();  // Recalculate column
}

void TextBuffer::move_to_column(int new_column) {
    // First move to row start
    move_to_row_start();
    
    // Then move forward to the desired column
    int current_col = 0;
    while (cursor != data.end() && *cursor != '\n' && current_col < new_column) {
        ++cursor;
        ++index;
        ++current_col;
    }
    
    column = current_col;
}

bool TextBuffer::up() {
    if (row == 1) {
        return false;
    }
    
    int target_column = column;
    
    // Move to start of current row
    move_to_row_start();
    
    // If we're at the beginning, can't go up
    if (cursor == data.begin()) {
        return false;
    }
    
    // Move one position back (to the newline ending the previous row)
    --cursor;
    --index;
    
    // Now we're at the newline, which is the end of the previous row
    // Update row (we're now at the end of the previous row)
    --row;
    
    // Move to the start of this (previous) row, then to target column
    move_to_column(target_column);
    
    return true;
}

bool TextBuffer::down() {
    // Move to end of current row
    move_to_row_end();
    
    // If we're at the end of the buffer, can't go down
    if (cursor == data.end()) {
        return false;
    }
    
    int target_column = column;
    
    // Skip the newline
    ++cursor;
    ++index;
    ++row;
    column = 0;
    
    // Move to the target column in the next row
    move_to_column(target_column);
    
    return true;
}

bool TextBuffer::is_at_end() const {
    return cursor == data.end();
}

char TextBuffer::data_at_cursor() const {
    return *cursor;
}

int TextBuffer::get_row() const {
    return row;
}

int TextBuffer::get_column() const {
    return column;
}

int TextBuffer::get_index() const {
    return index;
}

int TextBuffer::size() const {
    return data.size();
}

std::string TextBuffer::stringify() const {
    return std::string(data.begin(), data.end());
}

int TextBuffer::compute_column() const {
    if (cursor == data.begin()) {
        return 0;
    }
    
    auto it = cursor;
    int col = 0;
    
    // Move backward to find the start of the current row
    do {
        --it;
        if (*it == '\n') {
            break;
        }
        ++col;
    } while (it != data.begin());
    
    return col;
}