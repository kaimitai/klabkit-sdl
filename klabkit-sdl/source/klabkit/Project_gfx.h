#ifndef KLABLKIT_PROJECT_GFX_H
#define KLABLKIT_PROJECT_GFX_H

#include <SDL.h>
#include <array>
#include <map>
#include <vector>
#include "Project.h"
#include "../klib/Font.h"
#include "./../klib/Toast_notification_handler.h"

namespace kkit {

	constexpr std::size_t INDEX_WALL_TEXTURES{ 0 };
	constexpr std::size_t INDEX_APP_TEXTURES{ INDEX_WALL_TEXTURES + 1 };
	constexpr std::size_t INDEX_BG_TEXTURES{ INDEX_APP_TEXTURES + 1 };
	constexpr std::size_t INDEX_MM_TEXTURES{ INDEX_BG_TEXTURES + 1 }; // minimaps
	constexpr std::array<unsigned char, 4224> FONT_BYTES{ {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xF3, 0xCF, 0xF3, 0xCF, 0xFE, 0x7F, 0x83, 0xE7, 0xE1, 0xFF, 0xFE, 0x7F, 0xFF, 0x8F, 0xE3, 0xFF, 0xF3, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xE0, 0x0F, 0xFE, 0x7F, 0xE0, 0x0F, 0xE0, 0x07, 0xFF, 0x9F, 0x80, 0x03, 0xE0, 0x0F, 0x80, 0x07, 0xE0, 0x07, 0xE0, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0xF3, 0xFF, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0x80, 0x07, 0xE0, 0x0F, 0x80, 0x0F, 0x80, 0x03, 0x80, 0x03, 0xE0, 0x07, 0x9F, 0xF3, 0xF0, 0x0F, 0xFF, 0xF3, 0x9F, 0xF3, 0x9F, 0xFF, 0x9F, 0xF3, 0x9F, 0xF3, 0xE0, 0x0F, 0x80, 0x07, 0xE0, 0x0F, 0x80, 0x07, 0xE0, 0x07, 0x80, 0x03, 0x9F, 0xF3, 0x9F, 0xF3, 0x9F, 0xF3, 0x9F, 0xF3, 0x9F, 0xF3, 0x80, 0x03, 0xFE, 0x0F, 0x9F, 0xFF, 0xE1, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFE, 0x7F, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0x83, 0xFF, 0xFF, 0x9F, 0xFF, 0xFE, 0x7F, 0xFF, 0x9F, 0xF3, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF, 0xFE, 0x7F, 0xFE, 0x3F, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xF1, 0x8F, 0xF1, 0xCF, 0xFC, 0x7F, 0x83, 0xE3, 0xC0, 0xFF, 0xFE, 0x3F, 0xFF, 0x0F, 0xE1, 0xFF, 0xF1, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xC0, 0x03, 0xFC, 0x3F, 0xC0, 0x03, 0xC0, 0x03, 0xFF, 0x8F, 0x80, 0x01, 0xC0, 0x0F, 0x80, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0xF1, 0xFF, 0xC0, 0x07, 0xC0, 0x07, 0xC0, 0x03, 0x80, 0x03, 0xC0, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0xC0, 0x03, 0x8F, 0xF1, 0xF0, 0x0F, 0xFF, 0xF1, 0x8F, 0xE3, 0x8F, 0xFF, 0x87, 0xE1, 0x87, 0xF1, 0xC0, 0x03, 0x80, 0x03, 0xC0, 0x03, 0x80, 0x03, 0xC0, 0x03, 0x80, 0x03, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0x8F, 0xF1, 0x8F, 0xF3, 0x80, 0x01, 0xFE, 0x0F, 0x8F, 0xFF, 0xE0, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFE, 0x1F, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xFF, 0x8F, 0xFF, 0xFE, 0x3F, 0xFF, 0x8F, 0xF1, 0xFF, 0xF0, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xFF, 0xFE, 0x3F, 0xFC, 0x3F, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xF1, 0x8F, 0xF1, 0xCF, 0xF0, 0x07, 0x83, 0xE3, 0x80, 0x7F, 0xFE, 0x3F, 0xFE, 0x1F, 0xF1, 0xFF, 0xF9, 0x9F, 0xFE, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0x80, 0x03, 0xF8, 0x3F, 0x80, 0x03, 0x80, 0x03, 0xFF, 0x8F, 0x80, 0x03, 0x80, 0x1F, 0xC0, 0x03, 0x80, 0x03, 0x80, 0x03, 0xF0, 0x7F, 0xFF, 0xFF, 0xFE, 0x1F, 0xFF, 0xFF, 0xF8, 0xFF, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x07, 0x80, 0x07, 0x80, 0x03, 0x8F, 0xF1, 0xF8, 0x1F, 0xFF, 0xF1, 0x8F, 0xC3, 0x8F, 0xFF, 0x83, 0xC1, 0x83, 0xF1, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0xC0, 0x07, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0x8F, 0xF1, 0x8F, 0xF3, 0xC0, 0x01, 0xFE, 0x1F, 0x8F, 0xFF, 0xF0, 0xFF, 0xF8, 0x3F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFE, 0x07, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0x7F, 0xFF, 0xDF, 0xF1, 0xFF, 0xF8, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFE, 0x3F, 0xF8, 0x7F, 0x80, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xF1, 0x8F, 0xF1, 0xCF, 0xC0, 0x03, 0x83, 0xE3, 0x8C, 0x7F, 0xFE, 0x3F, 0xFE, 0x3F, 0xF8, 0xFF, 0xF8, 0x1F, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0x8F, 0xF1, 0xF0, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0x8F, 0x8F, 0xFF, 0x8F, 0xFF, 0xFF, 0xE3, 0x8F, 0xF1, 0x8F, 0xF1, 0xF0, 0x7F, 0xF0, 0x7F, 0xFC, 0x3F, 0xFF, 0xFF, 0xFC, 0x7F, 0x8F, 0xF3, 0x8F, 0xF3, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0xF1, 0x8F, 0x87, 0x8F, 0xFF, 0x81, 0x81, 0x81, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0xFC, 0x7F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0x8F, 0xF1, 0x8F, 0xE3, 0xFF, 0xF1, 0xFE, 0x3F, 0x8F, 0xFF, 0xF8, 0xFF, 0xF1, 0x1F, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0x8C, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xF3, 0xCF, 0xF1, 0xCF, 0x80, 0x03, 0x83, 0xC3, 0x8C, 0x7F, 0xFE, 0x7F, 0xFE, 0x3F, 0xF8, 0xFF, 0xFC, 0x3F, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0x8F, 0xE1, 0xF0, 0x3F, 0x9F, 0xF1, 0x9F, 0xF1, 0x8F, 0x8F, 0x8F, 0xFF, 0x8F, 0xFF, 0xFF, 0xE3, 0x8F, 0xF1, 0x8F, 0xF1, 0xF0, 0x7F, 0xF0, 0x7F, 0xF8, 0x7F, 0xFF, 0xFF, 0xFE, 0x3F, 0x9F, 0xF3, 0x8F, 0xF3, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF3, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0xF1, 0x8F, 0x0F, 0x8F, 0xFF, 0x80, 0x01, 0x80, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0xFC, 0x7F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0x87, 0xE3, 0x87, 0xC3, 0xFF, 0xE3, 0xFE, 0x3F, 0x87, 0xFF, 0xF8, 0xFF, 0xE3, 0x8F, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0x8C, 0x73, 0xFF, 0xFD, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0x80, 0x03, 0x8C, 0x7F, 0xFF, 0x87, 0x8C, 0x7F, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0x80, 0x03, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0x8F, 0xC1, 0xFE, 0x3F, 0xFF, 0xF1, 0xFF, 0xF1, 0x8F, 0x8F, 0x8F, 0xFF, 0x8F, 0xFF, 0xFF, 0xE3, 0x8F, 0xF1, 0x8F, 0xF1, 0xF0, 0x7F, 0xF0, 0x7F, 0xF0, 0xFF, 0x80, 0x03, 0xFF, 0x1F, 0xFF, 0xF3, 0x8F, 0x03, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0xF1, 0x8E, 0x1F, 0x8F, 0xFF, 0x88, 0x11, 0x88, 0x71, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0xFC, 0x7F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0xC3, 0xC7, 0xC3, 0x87, 0xFF, 0xC7, 0xFE, 0x3F, 0xC3, 0xFF, 0xF8, 0xFF, 0xC7, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0x80, 0x0F, 0xE0, 0x0F, 0xE0, 0x01, 0xE0, 0x0F, 0xF0, 0x0F, 0xE0, 0x03, 0x80, 0x0F, 0xF0, 0x7F, 0xFE, 0x1F, 0xF1, 0xF3, 0xFE, 0x3F, 0x81, 0x8F, 0x80, 0x0F, 0xE0, 0x0F, 0x80, 0x0F, 0xE0, 0x03, 0xF0, 0x07, 0xE0, 0x03, 0xF0, 0x01, 0x9F, 0xF3, 0x9F, 0xF3, 0x9F, 0xF3, 0x9F, 0xF3, 0x9F, 0xF3, 0x80, 0x03, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFE, 0x03, 0xE0, 0x09, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0x80, 0x03, 0x8C, 0x7F, 0xFF, 0x0F, 0x8C, 0x7F, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0x80, 0x03, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0x8F, 0x81, 0xFE, 0x3F, 0xFF, 0xE1, 0xFF, 0xF1, 0x8F, 0x8F, 0x8F, 0xFF, 0x8F, 0xFF, 0xFF, 0xC3, 0x8F, 0xF1, 0x8F, 0xF1, 0xF0, 0x7F, 0xF0, 0x7F, 0xE1, 0xFF, 0x80, 0x03, 0xFF, 0x8F, 0xFF, 0xE3, 0x8E, 0x03, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0xF1, 0x8C, 0x3F, 0x8F, 0xFF, 0x8C, 0x31, 0x8C, 0x31, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0xFC, 0x7F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0xE1, 0x8F, 0xE1, 0x0F, 0xFF, 0x8F, 0xFE, 0x3F, 0xE1, 0xFF, 0xF8, 0xFF, 0x8F, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x03, 0x80, 0x03, 0xC0, 0x03, 0xC0, 0x01, 0xC0, 0x03, 0xF0, 0x0F, 0xC0, 0x01, 0x80, 0x03, 0xF0, 0x3F, 0xFC, 0x0F, 0xF1, 0xE3, 0xFE, 0x3F, 0x80, 0x07, 0x80, 0x03, 0xC0, 0x03, 0x80, 0x03, 0xC0, 0x01, 0xF0, 0x03, 0xC0, 0x01, 0xF0, 0x01, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0x8F, 0xE3, 0x8F, 0xF1, 0x80, 0x03, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFE, 0x07, 0xC0, 0x01, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xC0, 0x07, 0x80, 0x1F, 0xFE, 0x1F, 0xC0, 0xFF, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xC0, 0x07, 0x80, 0x03, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xFF, 0x1F, 0x8F, 0x01, 0xFE, 0x3F, 0xFF, 0xC3, 0xFF, 0x03, 0x8F, 0x8F, 0x80, 0x1F, 0x80, 0x1F, 0xFF, 0x87, 0x80, 0x03, 0x80, 0x01, 0xFF, 0xFF, 0xF0, 0x7F, 0xC3, 0xFF, 0xC0, 0x07, 0xFF, 0xC7, 0xFF, 0xC3, 0x8E, 0x03, 0x80, 0x01, 0x80, 0x03, 0x8F, 0xFF, 0x8F, 0xF1, 0x80, 0x3F, 0x80, 0x3F, 0x8F, 0xC3, 0x80, 0x01, 0xFE, 0x3F, 0xFF, 0xF1, 0x88, 0x7F, 0x8F, 0xFF, 0x8E, 0xF1, 0x8E, 0x11, 0x8F, 0xF1, 0x80, 0x03, 0x8F, 0xF1, 0x80, 0x03, 0x80, 0x1F, 0xFC, 0x7F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0xF0, 0x1F, 0xF0, 0x1F, 0xFF, 0x1F, 0xFE, 0x3F, 0xF0, 0xFF, 0xF8, 0xFF, 0x9F, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x01, 0x80, 0x03, 0xF8, 0x1F, 0x80, 0x01, 0x80, 0x03, 0xF8, 0x3F, 0xFE, 0x0F, 0xF1, 0xC3, 0xFE, 0x3F, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x01, 0xF0, 0x03, 0x80, 0x03, 0xF8, 0x03, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0xC7, 0xC7, 0x8F, 0xF1, 0xC0, 0x07, 0xF8, 0x7F, 0xFE, 0x3F, 0xF1, 0xFF, 0xFF, 0x0F, 0x80, 0x03, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF1, 0xCF, 0xC0, 0x07, 0xFC, 0x3F, 0xE1, 0xFF, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xF8, 0x3F, 0x80, 0x03, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xFC, 0x3F, 0x8C, 0x31, 0xFE, 0x3F, 0xFF, 0x87, 0xFC, 0x03, 0x8F, 0x8F, 0x80, 0x07, 0x80, 0x07, 0xFF, 0x0F, 0xC0, 0x03, 0xC0, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF, 0x87, 0x8C, 0x73, 0x80, 0x01, 0x80, 0x03, 0x8F, 0xFF, 0x8F, 0xF1, 0x80, 0x1F, 0x80, 0x1F, 0x8F, 0x81, 0x80, 0x01, 0xFE, 0x3F, 0xFF, 0xF1, 0x80, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0x01, 0x8F, 0xF1, 0x80, 0x03, 0x8F, 0xF1, 0x80, 0x03, 0xC0, 0x07, 0xFC, 0x7F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8C, 0x73, 0xF8, 0x3F, 0xF8, 0x3F, 0xFC, 0x3F, 0xFE, 0x3F, 0xF8, 0x7F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0x8F, 0xF1, 0x87, 0xFE, 0x3F, 0x8C, 0x73, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0xF1, 0xF1, 0x8F, 0xFF, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8C, 0x73, 0xE3, 0x8F, 0x8F, 0xF1, 0xFF, 0x8F, 0xFC, 0x3F, 0xFE, 0x3F, 0xE1, 0xFF, 0xFF, 0xFF, 0x8F, 0xC1, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF1, 0xCF, 0xE0, 0x03, 0xF8, 0x7F, 0xC0, 0xFF, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xF8, 0x1F, 0xC0, 0x07, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xF8, 0x7F, 0x88, 0x71, 0xFE, 0x3F, 0xFF, 0x0F, 0xFE, 0x03, 0x8F, 0x8F, 0x80, 0x03, 0x80, 0x03, 0xFE, 0x1F, 0x80, 0x03, 0xE0, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF, 0x0F, 0x8C, 0x63, 0x80, 0x01, 0x80, 0x03, 0x8F, 0xFF, 0x8F, 0xF1, 0x80, 0x1F, 0x80, 0x1F, 0x8F, 0xC1, 0x80, 0x01, 0xFE, 0x3F, 0xFF, 0xF1, 0x81, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0x81, 0x8F, 0xF1, 0x80, 0x07, 0x8F, 0xF1, 0x80, 0x03, 0xE0, 0x03, 0xFC, 0x7F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8C, 0x73, 0xF8, 0x3F, 0xFC, 0x7F, 0xF8, 0x7F, 0xFE, 0x3F, 0xFE, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0x8F, 0xF1, 0x0F, 0xFE, 0x3F, 0x8C, 0x73, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0xF1, 0xF1, 0x8F, 0xFF, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8C, 0x73, 0xF1, 0x1F, 0x8F, 0xF1, 0xFF, 0x1F, 0xFC, 0x3F, 0xFE, 0x3F, 0xE1, 0xFF, 0xFF, 0xFF, 0x8F, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x03, 0xFC, 0x63, 0xF0, 0xFF, 0x80, 0x73, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xF1, 0x8F, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0x80, 0xF1, 0xFE, 0x3F, 0xFE, 0x1F, 0xFF, 0xF1, 0x80, 0x03, 0xFF, 0xF1, 0x8F, 0xF1, 0xFC, 0x3F, 0x8F, 0xF1, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xFF, 0x80, 0x03, 0xFF, 0xC7, 0xFE, 0x1F, 0x8E, 0x03, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0xF1, 0x80, 0x7F, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xC1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8E, 0xF1, 0x8F, 0xF1, 0xFF, 0xF1, 0xFC, 0x7F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8C, 0x73, 0xF0, 0x1F, 0xFC, 0x7F, 0xF0, 0xFF, 0xFE, 0x3F, 0xFF, 0x1F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x01, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xF1, 0x80, 0x01, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0x8F, 0xF0, 0x1F, 0xFE, 0x3F, 0x8C, 0x73, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0xF1, 0xFF, 0x80, 0x0F, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0x8C, 0x73, 0xF8, 0x3F, 0x8F, 0xF1, 0xFE, 0x3F, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0x8F, 0x11, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x03, 0xFC, 0x73, 0xE1, 0xFF, 0x8C, 0x23, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xF1, 0x8F, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xFF, 0x81, 0xF1, 0xFE, 0x3F, 0xFC, 0x3F, 0xFF, 0xF1, 0x80, 0x03, 0xFF, 0xF1, 0x8F, 0xF1, 0xF8, 0x7F, 0x8F, 0xF1, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xFF, 0x80, 0x03, 0xFF, 0x8F, 0xFC, 0x3F, 0x8F, 0x07, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0xF1, 0x8C, 0x3F, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xE1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8C, 0x21, 0x8F, 0xF1, 0xFF, 0xF1, 0xFC, 0x7F, 0x8F, 0xF1, 0x87, 0xE1, 0x8C, 0x73, 0xE1, 0x8F, 0xFC, 0x7F, 0xE1, 0xFF, 0xFE, 0x3F, 0xFF, 0x8F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x01, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xF1, 0x80, 0x01, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0x8F, 0xF0, 0x3F, 0xFE, 0x3F, 0x8C, 0x73, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0xF1, 0xFF, 0xC0, 0x03, 0xFE, 0x3F, 0x8F, 0xF1, 0x87, 0xE1, 0x8C, 0x73, 0xFC, 0x7F, 0x8F, 0xF1, 0xFC, 0x7F, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0x8E, 0x31, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x07, 0xFC, 0x63, 0xC3, 0xFF, 0x8E, 0x03, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFB, 0xDF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xFF, 0x83, 0xF1, 0xFE, 0x3F, 0xF8, 0x7F, 0xFF, 0xF1, 0xC0, 0x07, 0xFF, 0xF1, 0x8F, 0xF1, 0xF0, 0xFF, 0x8F, 0xF1, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xC0, 0x07, 0xFF, 0x1F, 0xFE, 0xFF, 0x8F, 0xDF, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0xF1, 0x8E, 0x1F, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8E, 0x03, 0x8F, 0xF1, 0xFF, 0xF1, 0xFC, 0x7F, 0x8F, 0xF1, 0x83, 0xC3, 0x8C, 0x73, 0xC3, 0xC7, 0xFC, 0x7F, 0xC3, 0xFF, 0xFE, 0x3F, 0xFF, 0xC7, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x01, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xF1, 0x80, 0x03, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0x8F, 0xF0, 0x1F, 0xFE, 0x3F, 0x8C, 0x73, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0xF1, 0xFF, 0xF8, 0x03, 0xFE, 0x3F, 0x8F, 0xF1, 0xC3, 0xC3, 0x8C, 0x73, 0xF8, 0x3F, 0x8F, 0xF1, 0xF8, 0xFF, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0x8C, 0x71, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF1, 0xCF, 0x80, 0x03, 0x87, 0x87, 0x8F, 0x07, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFC, 0x7F, 0xF0, 0x7F, 0xFF, 0xFF, 0xF0, 0x7F, 0x87, 0xFF, 0x8F, 0xF1, 0xFE, 0x3F, 0xF0, 0xFF, 0x8F, 0xF1, 0xFF, 0x8F, 0xFF, 0xF1, 0x8F, 0xF1, 0xE1, 0xFF, 0x8F, 0xF1, 0xFF, 0xF1, 0xF0, 0x7F, 0xF0, 0x7F, 0xF8, 0x7F, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0x9F, 0xF1, 0x8F, 0x0F, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0x07, 0x8F, 0xF1, 0x8F, 0xF1, 0xFC, 0x7F, 0x8F, 0xF1, 0xE1, 0x87, 0x8C, 0x73, 0x87, 0xE3, 0xFC, 0x7F, 0x87, 0xFF, 0xFE, 0x3F, 0xFF, 0xE3, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF3, 0x8F, 0xF1, 0x8F, 0xFF, 0xFE, 0x3F, 0x80, 0x01, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0x8F, 0xF0, 0x0F, 0xFE, 0x3F, 0x8C, 0x73, 0x8F, 0xF1, 0x8F, 0xF1, 0x80, 0x03, 0x80, 0x01, 0xF1, 0xFF, 0xFF, 0xF1, 0xFE, 0x3F, 0x8F, 0xF1, 0xE1, 0x87, 0x8C, 0x73, 0xF1, 0x1F, 0x80, 0x01, 0xF1, 0xFF, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0x88, 0xF1, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF1, 0xCF, 0x80, 0x07, 0x8F, 0x83, 0x8F, 0x07, 0xFF, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFE, 0x7F, 0xF0, 0x7F, 0xFF, 0xFF, 0xF0, 0x7F, 0x8F, 0xFF, 0x8F, 0xF1, 0xFE, 0x3F, 0xE1, 0xFF, 0x8F, 0xF1, 0xFF, 0x8F, 0xFF, 0xF1, 0x8F, 0xF1, 0xC3, 0xFF, 0x8F, 0xF1, 0xFF, 0xF1, 0xF0, 0x7F, 0xF0, 0x7F, 0xFC, 0x3F, 0xFF, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0xFE, 0x3F, 0x8F, 0xF1, 0x8F, 0x87, 0x8F, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0x8F, 0x07, 0x8F, 0xF1, 0x8F, 0xF1, 0xFC, 0x7F, 0x8F, 0xF1, 0xF0, 0x0F, 0x8C, 0x73, 0x8F, 0xF1, 0xFC, 0x7F, 0x8F, 0xFF, 0xFE, 0x3F, 0xFF, 0xF1, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xF1, 0x8F, 0xFF, 0xFE, 0x3F, 0xC0, 0x01, 0x8F, 0xF1, 0xFE, 0x3F, 0xFF, 0x8F, 0xF1, 0x87, 0xFE, 0x3F, 0x8C, 0x73, 0x8F, 0xF1, 0x8F, 0xF1, 0x80, 0x03, 0xC0, 0x01, 0xF1, 0xFF, 0xFF, 0xF1, 0xFE, 0x3F, 0x8F, 0xF1, 0xF0, 0x0F, 0x8C, 0x73, 0xE3, 0x8F, 0xC0, 0x01, 0xE3, 0xFF, 0xF8, 0xFF, 0xFE, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0x81, 0xF1, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF1, 0xCF, 0xC0, 0x1F, 0x8F, 0x83, 0x80, 0x03, 0xFF, 0xFF, 0xFE, 0x1F, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF0, 0x7F, 0x8F, 0xFF, 0x80, 0x03, 0xF8, 0x1F, 0xC0, 0x03, 0x80, 0x03, 0xFF, 0x8F, 0xC0, 0x03, 0x80, 0x03, 0x87, 0xFF, 0x80, 0x03, 0xC0, 0x03, 0xF0, 0x7F, 0xF0, 0x7F, 0xFE, 0x1F, 0xFF, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0x80, 0x07, 0x8F, 0xF1, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x07, 0x8F, 0xFF, 0x80, 0x03, 0x8F, 0xF1, 0xF8, 0x1F, 0x80, 0x03, 0x8F, 0xC3, 0x80, 0x07, 0x8F, 0xF1, 0x8F, 0xF1, 0x80, 0x03, 0x8F, 0xFF, 0x80, 0x03, 0x8F, 0xF1, 0x80, 0x03, 0xFC, 0x7F, 0x80, 0x03, 0xF8, 0x1F, 0x80, 0x03, 0x8F, 0xF1, 0xFC, 0x7F, 0x80, 0x03, 0xFE, 0x1F, 0xFF, 0xF1, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x01, 0x80, 0x03, 0x80, 0x03, 0x80, 0x01, 0x80, 0x1F, 0xFE, 0x3F, 0xE0, 0x01, 0x8F, 0xF1, 0xF8, 0x1F, 0xFF, 0x8F, 0xF1, 0xC3, 0xF8, 0x1F, 0x8C, 0x73, 0x8F, 0xF1, 0x80, 0x03, 0x80, 0x0F, 0xE0, 0x01, 0xF1, 0xFF, 0xC0, 0x03, 0xFE, 0x03, 0x80, 0x01, 0xF8, 0x1F, 0x80, 0x03, 0xC7, 0xC7, 0xE0, 0x01, 0xC0, 0x07, 0xF1, 0xFF, 0xFE, 0x3F, 0xF8, 0x7F, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF1, 0xCF, 0xFC, 0x7F, 0x8F, 0x83, 0xC0, 0x23, 0xFF, 0xFF, 0xFF, 0x0F, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF0, 0x7F, 0x8F, 0xFF, 0xC0, 0x03, 0xF0, 0x0F, 0x80, 0x01, 0xC0, 0x03, 0xFF, 0x8F, 0x80, 0x03, 0xC0, 0x03, 0x8F, 0xFF, 0xC0, 0x03, 0x80, 0x03, 0xF0, 0x7F, 0xF0, 0x7F, 0xFF, 0x1F, 0xFF, 0xFF, 0xF1, 0xFF, 0xFC, 0x7F, 0xC0, 0x03, 0x8F, 0xF1, 0x80, 0x03, 0xC0, 0x03, 0x80, 0x03, 0x80, 0x03, 0x8F, 0xFF, 0xC0, 0x03, 0x8F, 0xF1, 0xF0, 0x0F, 0xC0, 0x03, 0x8F, 0xE3, 0x80, 0x03, 0x8F, 0xF1, 0x8F, 0xF1, 0xC0, 0x03, 0x8F, 0xFF, 0xC0, 0x21, 0x8F, 0xF1, 0xC0, 0x03, 0xFC, 0x7F, 0xC0, 0x03, 0xFC, 0x3F, 0xC0, 0x07, 0x8F, 0xF1, 0xFC, 0x7F, 0x80, 0x01, 0xFE, 0x0F, 0xFF, 0xF1, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x01, 0x80, 0x03, 0xC0, 0x03, 0xC0, 0x01, 0xC0, 0x0F, 0xFE, 0x3F, 0xFF, 0xF1, 0x8F, 0xF1, 0xF0, 0x0F, 0x8F, 0x8F, 0xF1, 0xE3, 0xF0, 0x0F, 0x8C, 0x73, 0x8F, 0xF1, 0xC0, 0x03, 0x8F, 0xFF, 0xFF, 0xF1, 0xF1, 0xFF, 0x80, 0x03, 0xFF, 0x01, 0xC0, 0x01, 0xFC, 0x3F, 0xC0, 0x07, 0x8F, 0xE3, 0xFF, 0xF1, 0x80, 0x03, 0xE1, 0xFF, 0xFE, 0x3F, 0xFC, 0x3F, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF3, 0xCF, 0xFE, 0x7F, 0x9F, 0x87, 0xE0, 0x73, 0xFF, 0xFF, 0xFF, 0x8F, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF0, 0x7F, 0x9F, 0xFF, 0xE0, 0x07, 0xF0, 0x0F, 0x80, 0x03, 0xE0, 0x0F, 0xFF, 0x9F, 0x80, 0x0F, 0xE0, 0x0F, 0x9F, 0xFF, 0xE0, 0x0F, 0x80, 0x07, 0xF0, 0x7F, 0xF0, 0x7F, 0xFF, 0x9F, 0xFF, 0xFF, 0xF3, 0xFF, 0xFE, 0x7F, 0xE0, 0x03, 0x9F, 0xF3, 0x80, 0x0F, 0xE0, 0x07, 0x80, 0x07, 0x80, 0x03, 0x9F, 0xFF, 0xE0, 0x0F, 0x9F, 0xF3, 0xF0, 0x0F, 0xE0, 0x07, 0x9F, 0xF3, 0x80, 0x03, 0x9F, 0xF3, 0x9F, 0xF3, 0xE0, 0x07, 0x9F, 0xFF, 0xE0, 0x73, 0x9F, 0xF3, 0xE0, 0x07, 0xFE, 0x7F, 0xE0, 0x07, 0xFE, 0x7F, 0xE1, 0x0F, 0x9F, 0xF3, 0xFE, 0x7F, 0x80, 0x03, 0xFE, 0x0F, 0xFF, 0xF3, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x03, 0x80, 0x0F, 0xE0, 0x0F, 0xE0, 0x03, 0xE0, 0x0F, 0xFE, 0x7F, 0xFF, 0xF1, 0x9F, 0xF3, 0xF0, 0x0F, 0x8F, 0x8F, 0xF3, 0xF3, 0xF0, 0x0F, 0x9E, 0x73, 0x9F, 0xF3, 0xE0, 0x0F, 0x8F, 0xFF, 0xFF, 0xF1, 0xF3, 0xFF, 0x80, 0x0F, 0xFF, 0x81, 0xE0, 0x03, 0xFE, 0x7F, 0xE1, 0x0F, 0x9F, 0xF3, 0xFF, 0xF1, 0x80, 0x03, 0xE3, 0xFF, 0xFE, 0x7F, 0xFE, 0x3F, 0xFF, 0xFF, 0x80, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} };

	class Project_gfx {

		klib::Font font;
		klib::Toast_notification_handler toasts;
		std::map<int, std::vector<SDL_Texture*>> textures;
		SDL_Color floor_color;

		void load_background_textures(SDL_Renderer* p_rnd);

	public:
		Project_gfx(SDL_Renderer* p_rnd, const kkit::Project& p_project);
		~Project_gfx(void);

		void reload_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_frame_no);
		void reload_minimap_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_frame_no);

		SDL_Texture* get_tile_texture(int p_frame_no) const;
		SDL_Texture* get_app_texture(int p_frame_no) const;
		SDL_Texture* get_bg_texture(int p_frame_no) const;
		SDL_Texture* get_minimap_texture(int p_frame_no) const;
		SDL_Color get_floor_color(void) const;
		const klib::Font& get_font(void) const;

		void move(int p_delta_ms);
		void draw(SDL_Renderer* p_rnd) const;

		void add_toast_ok(const std::string& p_msg);
		void add_toast_info(const std::string& p_msg);
		void add_toast_error(const std::string& p_msg);
	};

}

#endif
