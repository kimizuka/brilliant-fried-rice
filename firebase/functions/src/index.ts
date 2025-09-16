import * as functions from "firebase-functions";
import * as admin from "firebase-admin";

admin.initializeApp();

const db = admin.firestore();

export const toggleFriedRiceState = functions.https.onRequest(
  async (req, res): Promise<void> => {
    if (req.method !== "POST") {
      res
        .status(405)
        .send("Method Not Allowed. Only POST requests are accepted.");
      return;
    }

    const isOpenDocRef = db.collection("brilliant-fried-rice").doc("state");

    try {
      await db.runTransaction(async (transaction) => {
        const doc = await transaction.get(isOpenDocRef);

        let newIsOpenState: boolean;

        if (!doc.exists) {
          functions.logger.info(
            "Document 'brilliant-fried-rice' does not exist. " +
            "Initializing to closed (false)."
          );
          newIsOpenState = false;
        } else {
          const currentData = doc.data();

          if (currentData && typeof currentData.isOpen === "boolean") {
            newIsOpenState = !currentData.isOpen;
            functions.logger.info(
              `Toggling lid state from ${currentData.isOpen} ` +
              `to ${newIsOpenState}`
            );
          } else {
            functions.logger.warn(
              "`isOpen` field missing or not a boolean. " +
              "Initializing to closed (false).",
              {currentData}
            );
            newIsOpenState = false;
          }
        }

        transaction.set(isOpenDocRef, {isOpen: newIsOpenState});
      });

      res.status(200).send("Fried rice state toggled successfully!");
      return;
    } catch (error) {
      functions.logger.error("Error toggling fried rice state:", error);
      res.status(500).send("Error toggling fried rice state.");
      return;
    }
  }
);

export const getFriedRiceState = functions.https.onRequest(
  async (req, res): Promise<void> => {
    if (req.method !== "GET") {
      res
        .status(405)
        .send("Method Not Allowed. Only GET requests are accepted.");
      return;
    }

    const isOpenDocRef = db.collection("brilliant-fried-rice").doc("state");

    try {
      const doc = await isOpenDocRef.get();

      if (!doc.exists) {
        functions.logger.info(
          "Document 'brilliant-fried-rice' does not exist. " +
          "Assuming closed (false)."
        );
        res.status(200).json({isOpen: false});
        return;
      }

      const data = doc.data();

      if (data && typeof data.isOpen === "boolean") {
        res.status(200).json({isOpen: data.isOpen});
        return;
      } else {
        functions.logger.warn(
          "`isOpen` field missing or not a boolean. " +
          "Assuming closed (false).",
          {data}
        );
        res.status(200).json({isOpen: false});
        return;
      }
    } catch (error) {
      functions.logger.error("Error fetching fried rice state:", error);
      res.status(500).send("Error fetching fried rice state.");
      return;
    }
  }
);

export const openFriedRice = functions.https.onRequest(
  async (req, res): Promise<void> => {
    if (req.method !== "POST") {
      res
        .status(405)
        .send("Method Not Allowed. Only POST requests are accepted.");
      return;
    }

    const isOpenDocRef = db.collection("brilliant-fried-rice").doc("state");

    try {
      await isOpenDocRef.set({isOpen: true});
      res.status(200).send("Fried rice is now open!");
      return;
    } catch (error) {
      functions.logger.error("Error opening fried rice:", error);
      res.status(500).send("Error opening fried rice.");
      return;
    }
  }
);

export const closeFriedRice = functions.https.onRequest(
  async (req, res): Promise<void> => {
    if (req.method !== "POST") {
      res
        .status(405)
        .send("Method Not Allowed. Only POST requests are accepted.");
      return;
    }

    const isOpenDocRef = db.collection("brilliant-fried-rice").doc("state");

    try {
      await isOpenDocRef.set({isOpen: false});
      res.status(200).send("Fried rice is now closed!");
      return;
    } catch (error) {
      functions.logger.error("Error closing fried rice:", error);
      res.status(500).send("Error closing fried rice.");
      return;
    }
  }
);
