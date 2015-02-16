SET UNIQUE_CHECKS=1;
SET FOREIGN_KEY_CHECKS=1;

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `clusterDB` ;
USE `clusterDB` ;

-- -----------------------------------------------------
-- Table `clusterDB`.`clients`
-- -----------------------------------------------------

CREATE  TABLE IF NOT EXISTS `clients` (
  `id` INT NOT NULL AUTO_INCREMENT ,
  `username` VARCHAR(45) NOT NULL ,
  `password` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;

CREATE UNIQUE INDEX `userUNIQ` ON `clients` (`username`) ;

-- -----------------------------------------------------
-- Table `clusterDB`.`jobs`
-- -----------------------------------------------------

CREATE  TABLE IF NOT EXISTS `jobs` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `idClient` INT NOT NULL ,
  `name` VARCHAR(45) NOT NULL ,
  `jobType` VARCHAR(45) NOT NULL ,
  `neededWorkers` INT NOT NULL ,
  `args` VARCHAR(255) DEFAULT NULL,
  `path` VARCHAR(255) NOT NULL ,
  `loaded` BOOL DEFAULT 0,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `existanceOfClient`
    FOREIGN KEY (`idClient`)
    REFERENCES `clients` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;

CREATE UNIQUE INDEX `jobUNIQ` ON `jobs` (`idClient`, `name`) ;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;