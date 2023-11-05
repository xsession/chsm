# tests/conftest.py
import pytest
from selenium import webdriver
from eel import app  # Assuming your Eel app is in a module named 'app.py' under the 'eel' directory

@pytest.fixture(scope='session')
def eel_app():
    # Start your Eel app here, for example:
    app.start_app()
    yield  # This is where the testing happens
    # Any teardown code can go here

@pytest.fixture
def browser():
    # Setup
    driver = webdriver.Chrome()
    yield driver
    # Teardown
    driver.quit()
