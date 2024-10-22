from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.wait import WebDriverWait
from selenium.common import NoSuchElementException, ElementNotInteractableException
from pandas import DataFrame
import argparse
import time


parser = argparse.ArgumentParser(prog='phones.py')
parser.add_argument('query')
args = parser.parse_args()

driver = webdriver.Firefox()
driver.get(f'https://www.11888.gr/white-pages/?query={args.query}')
details = driver.find_elements(By.XPATH,'//div[@class="useful-results__item"]//a[@class="details-button"]') 

errors = [NoSuchElementException, ElementNotInteractableException]

wait = WebDriverWait(driver, timeout=2, poll_frequency=.2, ignored_exceptions=errors)
wait.until(lambda d : all([elem.is_displayed() for elem in details]))

requests = [elem.get_attribute("href") for elem in details]

page=0
finished=False
while not finished:
    try:
        page = page + 1
        next_page=driver.find_element(By.XPATH, '//li[@class="page-item load-next-arrow"]')
        driver.get(f'https://www.11888.gr/white-pages/?query={args.query}&page={page}')
        time.sleep(1)
        details = driver.find_elements(By.XPATH,'//div[@class="useful-results__item"]//a[@class="details-button"]') 
        wait = WebDriverWait(driver, timeout=5, poll_frequency=2, ignored_exceptions=errors)
        wait.until(lambda d : all([elem.is_displayed() for elem in details]))
        if len(details) == 0:
            finished = True
        requests.extend([elem.get_attribute("href") for elem in details])
    except NoSuchElementException:
        finished=True

addresses=[]
numbers=[]
for request in requests:
    driver.get(request)
    time.sleep(1)
    loc = driver.find_element(By.XPATH, '//div[@class="location"]/div[@class="address"]')
    phones = driver.find_elements(By.XPATH, '//div[@class="phones"]//div[@class="desktop"]')
    wait = WebDriverWait(driver, timeout=5, poll_frequency=2, ignored_exceptions=errors)
    wait.until(lambda d : all([p.is_displayed for p in phones]) and loc.is_displayed())
    
    addresses.append(loc.text)
    numbers.extend([phone.text for phone in phones])
    if len(phones) > 0:
        addresses.extend([''] * (len(phones) - 1))

df = DataFrame({'Address': addresses, 'Phone': numbers})
df.to_excel(f'{args.query}.xlsx')

driver.quit()

