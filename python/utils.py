"""
utils.py — Shared helper functions for the Mini VCS Python analytics layer.
"""

from datetime import datetime


def parse_timestamp(ts_str):
    """Parse 'YYYY-MM-DD HH:MM:SS' into a datetime object. Returns None on failure."""
    for fmt in ("%Y-%m-%d %H:%M:%S", "%Y-%m-%d %H:%M"):
        try:
            return datetime.strptime(ts_str, fmt)
        except ValueError:
            continue
    return None


def short_id(commit_id):
    """Return the first 7 characters of a commit ID."""
    return commit_id[:7] if commit_id else ""


def format_size(kb):
    """Format size in KB to a human-readable string."""
    if kb >= 1024:
        return f"{kb / 1024:.1f} MB"
    return f"{kb:.1f} KB"
