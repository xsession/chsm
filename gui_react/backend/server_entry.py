"""
Entry point for running the CHSM backend under Electron (or standalone).
Reads CHSM_BACKEND_PORT from the environment (default 8000).
"""
import os
import uvicorn

if __name__ == "__main__":
    port = int(os.environ.get("CHSM_BACKEND_PORT", "8000"))
    uvicorn.run(
        "server:app",
        host="127.0.0.1",
        port=port,
        log_level="info",
    )
