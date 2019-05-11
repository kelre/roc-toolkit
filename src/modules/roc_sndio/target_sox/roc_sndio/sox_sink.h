/*
 * Copyright (c) 2015 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @file roc_sndio/target_sox/roc_sndio/sox_sink.h
//! @brief SoX sink.

#ifndef ROC_SNDIO_SOX_SINK_H_
#define ROC_SNDIO_SOX_SINK_H_

#include <sox.h>

#include "roc_core/iallocator.h"
#include "roc_core/noncopyable.h"
#include "roc_core/stddefs.h"
#include "roc_core/unique_ptr.h"
#include "roc_packet/units.h"
#include "roc_sndio/isink.h"

namespace roc {
namespace sndio {

//! SoX sink.
//! @remarks
//!  Writes samples to output file or device.
//!  Supports multiple drivers for different file types and audio systems.
class SoxSink : public ISink, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @b Parameters
    //!  - @p channels defines bitmask of enabled channels in input buffers
    //!  - @p sample_rate defines sample rate of input buffers
    SoxSink(core::IAllocator& allocator,
            packet::channel_mask_t channels,
            size_t sample_rate,
            size_t frame_size);

    virtual ~SoxSink();

    //! Open output file or device.
    //!
    //! @b Parameters
    //!  - @p driver is output driver name;
    //!  - @p output is output file or device name, "-" for stdout.
    //!
    //! @remarks
    //!  If @p driver or @p output are NULL, defaults are used.
    //!
    //! @pre
    //!  Should be called once before calling start().
    bool open(const char* driver, const char* output);

    //! Get sample rate of an output file or a device.
    //!
    //! @pre
    //!  Output file or device should be opened.
    size_t sample_rate() const;

    //! Returns true if output is a real file.
    //!
    //! @pre
    //!  Output file or device should be opened.
    bool is_file() const;

    //! Returns recommended frame size.
    size_t frame_size() const;

    //! Write audio frame.
    virtual void write(audio::Frame& frame);

private:
    bool prepare_();
    bool open_(const char* driver, const char* output);
    void write_(const sox_sample_t* samples, size_t n_samples);
    void close_();

    sox_format_t* output_;
    sox_signalinfo_t out_signal_;

    core::IAllocator& allocator_;

    core::UniquePtr<sox_sample_t> buffer_;
    size_t buffer_size_;

    bool is_file_;
};

} // namespace sndio
} // namespace roc

#endif // ROC_SNDIO_SOX_SINK_H_
