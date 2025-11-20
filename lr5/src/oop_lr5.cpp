#include "../include/oop_lr5.hpp"

#include <algorithm>
#include <new>
#include <stdexcept>

namespace oop {

namespace {
constexpr std::size_t normalize_alignment(std::size_t alignment) noexcept {
    return alignment == 0 ? alignof(std::max_align_t) : alignment;
}

void delete_block(void* ptr, std::size_t alignment) noexcept {
    if (!ptr) {
        return;
    }

    if (alignment > alignof(std::max_align_t)) {
        ::operator delete(ptr, std::align_val_t(alignment));
    } else {
        ::operator delete(ptr);
    }
}
}  // namespace

VectorTrackingMemoryResource::~VectorTrackingMemoryResource() {
    release_all();
}

std::size_t VectorTrackingMemoryResource::tracked_blocks() const noexcept {
    return blocks_.size();
}

std::size_t VectorTrackingMemoryResource::reusable_blocks() const noexcept {
    return tracked_blocks() - active_blocks();
}

std::size_t VectorTrackingMemoryResource::active_blocks() const noexcept {
    return static_cast<std::size_t>(std::count_if(blocks_.begin(), blocks_.end(),
                                                  [](const BlockInfo& block) { return block.in_use; }));
}

void* VectorTrackingMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
    const std::size_t normalized_alignment = normalize_alignment(alignment);

    auto reusable_block = std::find_if(
        blocks_.begin(), blocks_.end(), [&](const BlockInfo& block) {
            return !block.in_use && block.size >= bytes && block.alignment >= normalized_alignment;
        });

    if (reusable_block != blocks_.end()) {
        reusable_block->in_use = true;
        reusable_block->size = bytes;
        reusable_block->alignment = normalized_alignment;
        return reusable_block->ptr;
    }

    void* ptr = nullptr;
    if (normalized_alignment > alignof(std::max_align_t)) {
        ptr = ::operator new(bytes, std::align_val_t(normalized_alignment));
    } else {
        ptr = ::operator new(bytes);
    }

    blocks_.push_back(BlockInfo{ptr, bytes, normalized_alignment, true});
    return ptr;
}

void VectorTrackingMemoryResource::do_deallocate(void* p, std::size_t, std::size_t) {
    if (!p) {
        return;
    }

    auto block = std::find_if(blocks_.begin(), blocks_.end(),
                              [p](const BlockInfo& info) { return info.ptr == p; });
    if (block == blocks_.end()) {
        throw std::invalid_argument("Attempted to deallocate memory not owned by resource");
    }

    block->in_use = false;
}

bool VectorTrackingMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

void VectorTrackingMemoryResource::release_all() noexcept {
    for (auto& block : blocks_) {
        delete_block(block.ptr, block.alignment);
        block.ptr = nullptr;
    }
    blocks_.clear();
}

}  // namespace oop
