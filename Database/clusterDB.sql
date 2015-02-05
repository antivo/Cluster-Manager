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

-- -----------------------------------------------------
-- Table `clusterDB`.`unfinishedJobs`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `unfinishedJobs` (
  `idJob` INT NOT NULL,
  `parallel` BOOL NOT NULL,
  `neededWorkers` INT NOT NULL ,
  `workersToCall` INT NOT NULL DEFAULT 0, -- trigger `initWorkersToCall` -> it will never be 0
  `parameters` VARCHAR(255) DEFAULT NULL,
  `path` VARCHAR(255) NOT NULL ,
  `ready` BOOL NOT NULL , 
  `executed` BOOL DEFAULT NULL,
  `killed` BOOL DEFAULT 0,
  PRIMARY KEY (`idJob`) ,
  CONSTRAINT `existanceOfJob`
    FOREIGN KEY (`idJob`)
    REFERENCES `jobs` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;

delimiter |

CREATE TRIGGER initWorkersToCall BEFORE INSERT ON `unfinishedJobs`
  FOR EACH ROW BEGIN
    SET NEW.`workersToCall` = NEW.`neededWorkers`;
  END;
|

delimiter ;

-- -----------------------------------------------------
-- Table `clusterDB`.`workers`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `workers` (
  `idUnfinishedJob` INT NOT NULL ,
  `ip_adress` VARCHAR(255) NOT NULL ,
  `directory` VARCHAR(255) DEFAULT NULL,
  `retrive` BOOL DEFAULT NULL,
  PRIMARY KEY (`idUnfinishedJob`, `ip_adress`) ,
  CONSTRAINT `existanceOfUnfinishedJob`
    FOREIGN KEY (`idUnfinishedJob`)
    REFERENCES `unfinishedJobs` (`idJob`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


delimiter |

CREATE TRIGGER decWorkersToCall AFTER INSERT ON `workers`
  FOR EACH ROW BEGIN
    UPDATE `unfinishedJobs` SET `unfinishedJobs`.`workersToCall` = `unfinishedJobs`.`workersToCall` - 1 
            WHERE `unfinishedJobs`.`idJob` = NEW.`idUnfinishedJob`;
    UPDATE `unfinishedJobs` SET `unfinishedJobs`.`executed` = 0 
            WHERE `unfinishedJobs`.`idJob` = NEW.`idUnfinishedJob` AND `unfinishedJobs`.`workersToCall` = 0;
  END;
|

delimiter ;

-- -----------------------------------------------------
-- Table `clusterDB`.`finishedJobs`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `finishedJobs` (
  `idJob` INT NOT NULL ,
  `parallel` BOOL NOT NULL ,
  `numOfWorkers` INT NOT NULL , 
  `killed` BOOL ,
  PRIMARY KEY (`idJob`) ,
  CONSTRAINT `existanceOfJob2`
    FOREIGN KEY (`idJob`)
    REFERENCES `jobs` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;

delimiter |

CREATE TRIGGER initFinishedParallel BEFORE DELETE ON `unfinishedJobs`
  FOR EACH ROW BEGIN
    INSERT INTO `finishedJobs` (`idJob`, `parallel`, `numOfWorkers`, `killed`) VALUES (OLD.`idJob`, OLD.`parallel`, (OLD.`neededworkers` - OLD.`workersToCall`) , OLD.`killed`);  
  END;

|

delimiter ;

-- -----------------------------------------------------
-- Table `clusterDB`.`resourceCount`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `resourceCount` (
  `idClient` INT NOT NULL ,
  `num_workers` INT NOT NULL DEFAULT 0 ,
  PRIMARY KEY (`idClient`) ,
  CONSTRAINT `existanceOfClient4`
    FOREIGN KEY (`idClient`)
    REFERENCES `clients` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;

delimiter |

CREATE TRIGGER resourceCountINIT AFTER INSERT ON `clients`
  FOR EACH ROW BEGIN
    INSERT INTO `resourceCount` (`idClient`, `num_workers`) VALUES (NEW.`id`, 0);
  END;
|

CREATE TRIGGER resourceCountINC BEFORE INSERT ON `workers`
  FOR EACH ROW BEGIN
    UPDATE `resourceCount` SET `num_workers` = `num_workers` + 1 WHERE `idClient` = (SELECT DISTINCT `idClient` FROM `jobs` WHERE `id` = NEW.`idUnfinishedJob`);
  END;
|

CREATE TRIGGER resourceCountDEC BEFORE DELETE ON `workers`
  FOR EACH ROW BEGIN
    UPDATE `resourceCount` SET `num_workers` = `num_workers` - 1 WHERE `idClient` = (SELECT DISTINCT `idClient` FROM `jobs` WHERE `id` = OLD.`idUnfinishedJob`);
 END;
|

delimiter ;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;